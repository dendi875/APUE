/**
 * 该程序使用多线程守护进程来读取配置文件
 *
 * 1、创建一个守护进程
 * 2、使用文件和记录锁来保证只运行一个守护进程的实例
 * 3、使用多线程和sigwait函数来重新读取配置文件
 */

#include "apue.h"
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int lockfile(int fd)
{
    struct flock fl;

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    return(fcntl(fd, F_SETLK, &fl));
}

int already_running(void)
{
    int     fd;
    char    buf[16];

    fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
    if (fd < 0) {
        syslog(LOG_ERR, "can't open %s：%s", LOCKFILE, strerror(errno));
        exit(1);
    }
    if (lockfile(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return(1);  /* 守护进程正在运行中 */
        }
        syslog(LOG_ERR, "can't lock %s：%s", LOCKFILE, strerror(errno));
        exit(1);
    }
    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    return(0);
}

void daemonize(const char *cmd)
{
    int                 i, fd0, fd1, fd2;
    pid_t               pid;
    struct rlimit       rl;
    struct sigaction    act;

    /**
     * 重新设置文件的权限掩码
     */
    umask(0);

    /**
     * 获取当前进程能够生成的最大文件描述符数量
     */
     if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        err_sys("%s：can't get file limit", cmd);
     }

     /**
      * 创建一个子进程，子进程创建一个会话，使子进程成为新会话的会话首进程并脱离控制终端
      */
    if ((pid = fork()) < 0) {
        err_sys("%s：fork error", cmd);
    } else if (pid > 0) {   /* 父进程 */
        exit(0);
    }
    setsid();

    /**
     * 1、忽略SIGHUP信号
     * 2、再次fork，防止将来调用open且没有指定O_NOCTTY时获取控制终端
     */
    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGHUP, &act, NULL) < 0) {
        err_sys("%s：can't ignore SIGHUP", cmd);
    }
    if ((pid = fork()) < 0) {
        err_sys("%s：can't fork", cmd);
    } else if (pid > 0) {
        exit(0);
    }

    /**
     * 该变当前工作目录
     */
    if (chdir("/") < 0) {
        err_quit("%s：can't change directory to /", cmd);
    }
    /**
     * 关闭所有的文件描述符
     */
    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }
    for (i = 0; i < rl.rlim_max; i++) {
        close(i);
    }

    /**
     * 使0、1、2文件描述符指向 /dev/null
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /**
     * 初始化日志文件
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d",
            fd0, fd1, fd2);
        exit(1);
    }
}

sigset_t    mask;

void reread(void)
{
    /* 重新读取配置文件代码 */
}

void *thr_fun(void *arg)
{
    int     err, signo;

    for (;;) {
        err = sigwait(&mask, &signo);
        if (err != 0) {
            syslog(LOG_ERR, "sigwait failed");
            exit(1);
        }

        switch (signo) {
            case SIGHUP:
                syslog(LOG_INFO, "Re-reading configuration file");
                reread();
                break;

            case SIGTERM:
                syslog(LOG_INFO, "got SIGTERM; exiting");
                exit(0);    /* 导致主控线程（守护进程也终止）*/
                break;
            default:
                syslog(LOG_INFO, "unexpected signal %d\n", signo);
        }
    }

    return((void *)0);
}


int main(int argc, char *argv[])
{
    int                 err;
    pthread_t           tid;
    char                *cmd;
    struct sigaction    sa;

    if ((cmd = strrchr(argv[0], '/')) == NULL) {    /* testdaemon被PATH变量中 */
        cmd = argv[0];
    } else {    /* ./testdaemon */
        cmd++;
    }

    /**
     * 变成一个守护进程
     */
    daemonize(cmd);

    /**
     * 确保只有一个守护进程实例在运行
     */
    if (already_running()) {
        syslog(LOG_ERR, "daemon already running");
        exit(1);
    }

    /**
     * 恢复 SIGHUP 的默认处理动作，并且阻塞所有信号
     */
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        err_quit("%s：can't restore SIGHUP default");
    }
    sigfillset(&mask);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0) {
        err_exit(err, "SIG_BLOCK error");
    }

    /**
     * 创建一个线程，让它去等待 SIGHUP、SIGTERM信号的出现，处理这两个信号
     */
    err = pthread_create(&tid, NULL, thr_fun, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread");
    }

    /**
     * 守护进程做其它事情
     */
    /* ... */
    while (1) {
        sleep(1);
    }

    exit(0);
}


/*
实验：
[root@localhost Chapter-13-Daemon Processes]# ./reread

查看进程
[root@localhost Chapter-13-Daemon Processes]# ps aux | grep reread
root     14961  0.0  0.0  12352   412 ?        Sl   14:12   0:00 ./reread
root     15069  0.0  0.0   5976   752 pts/0    S+   14:16   0:00 grep --color=auto reread
查看线程
[root@localhost Chapter-13-Daemon Processes]# ps -Lwf 14961
UID        PID  PPID   LWP  C NLWP STIME TTY      STAT   TIME CMD
root     14961     1 14961  0    2 14:12 ?        Sl     0:00 ./reread
root     14961     1 14962  0    2 14:12 ?        Sl     0:00 ./reread

给进程发送SIGUSR1信号
[root@localhost Chapter-13-Daemon Processes]# kill -SIGUSR1 14961
再次查看进程
[root@localhost Chapter-13-Daemon Processes]# ps aux | grep reread
root     14961  0.0  0.0  13376   584 ?        Sl   14:12   0:00 ./reread
root     15086  0.0  0.0   5976   748 pts/0    S+   14:18   0:00 grep --color=auto reread
再次查看线程
[root@localhost Chapter-13-Daemon Processes]# ps -Lwf 14961
UID        PID  PPID   LWP  C NLWP STIME TTY      STAT   TIME CMD
root     14961     1 14961  0    2 14:12 ?        Sl     0:00 ./reread
root     14961     1 14962  0    2 14:12 ?        Sl     0:00 ./reread

查看syslogd记录的日志
[root@localhost Chapter-13-Daemon Processes]# tail /var/log/messages
Apr 26 14:13:46 localhost dhclient[1998]: DHCPREQUEST on eth0 to 192.168.100.254 port 67 (xid=0x507ae328)
Apr 26 14:13:46 localhost dhclient[1998]: DHCPACK from 192.168.100.254 (xid=0x507ae328)
Apr 26 14:13:46 localhost NetworkManager[1928]: <info> (eth0): DHCPv4 state changed renew -> renew
Apr 26 14:13:46 localhost NetworkManager[1928]: <info>   address 192.168.100.142
Apr 26 14:13:46 localhost NetworkManager[1928]: <info>   prefix 24 (255.255.255.0)
Apr 26 14:13:46 localhost NetworkManager[1928]: <info>   gateway 192.168.100.2
Apr 26 14:13:46 localhost NetworkManager[1928]: <info>   nameserver '192.168.100.2'
Apr 26 14:13:46 localhost NetworkManager[1928]: <info>   domain name 'localdomain'
Apr 26 14:13:46 localhost dhclient[1998]: bound to 192.168.100.142 -- renewal in 886 seconds.
Apr 26 14:18:51 localhost reread: unexpected signal 10


可以再给进程发送SIGHUP信号和SIGTERM信号
*/