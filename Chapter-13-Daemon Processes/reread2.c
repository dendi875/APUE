/**
 * 该程序说明使用单线程守护进程来捕捉SIGHUP信号并重新读取配置文件
 *
 * 1、初始化一个守护进程
 * 2、使用文件和记录锁来保证只有一个守护进程的实例在运行
 * 3、注册 SIGHUP信号和SIGTERM信号，在SIGHUP信号处理函数中重新读取配置文件，在SIGTERM信号处理函数中
 * 终止进程
 */

#include "apue.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>

#define LOCKFILE  "/var/run/daemon.pid"
#define LOCKMODE  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int lockfile(int fd)
{
    struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    return(fcntl(fd, F_SETLK, &lock));
}

int already_running(void)
{
    int     fd;
    char    buf[16];

    /* 打开进程ID锁文件 */
    fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
    if (fd < 0) {
        syslog(LOG_ERR, "can't open %s：%s", LOCKFILE, strerror(errno));
        exit(1);
    }
    /**
     * 试图加锁
     */
    if (lockfile(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return(1);
        }
        syslog(LOG_ERR, "can't lock %s：%s", LOCKFILE, strerror(errno));
        exit(1);
    }

    /**
     *清除锁文件内容，把当前进程ID写进去
     */
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
    struct sigaction    sa;

    /**
     * 重新设置文件权限掩码值
     */
    umask(0);

    /**
     * 获取当前进程能够生成的最大文件描述符数量
     */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        err_sys("%s：can't get file limit", cmd);
    }

    /**
     * 创建子进程，子进程创建一个新会话
     */
    if ((pid = fork()) < 0) {
        err_sys("%s：fork error", cmd);
    } else if (pid > 0) {  /*　父进程退出 */
        exit(0);
    }
    setsid();

    /**
     * 忽略SIGHUP信号，再次fork
     */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        err_sys("%s：can't ignore SIGHUP", cmd);
    }
    if ((pid = fork()) < 0) {
        err_sys("%s：fork error", cmd);
    } else if (pid > 0) {
        exit(0);
    }

    /**
     * 改变当前工作目录
     */
    if (chdir("/") < 0) {
        err_quit("%s：can't change directory to /", cmd);
    }

    /**
     * 关闭所有文件描述符
     */
    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }
    for (i = 0; i < rl.rlim_max; i++) {
        close(i);
    }

    /**
     * 使0、1、2文件描述符都指向 /dev/null
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /**
     * 记录出错日志
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);  /* 与日志记录器（rsyslog）建立连接，并设置连接选项 */
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "%s：unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}


void reread(void)
{
    /* 重新读取配置文件 */
}

void sig_hup(int signo)
{
    syslog(LOG_INFO, "Re-reading configuration file");
    reread();
}

void sig_term(int signo)
{
    syslog(LOG_INFO, "got SIGTERM; exiting");
    exit(0);
}

int main(int argc, char *argv[])
{
    char                *cmd;
    struct sigaction    sa;

    if ((cmd = strrchr(argv[0], '/')) == NULL) {
        cmd = argv[0];
    } else {
        cmd++;
    }

    /**
     * 成为守护进程
     */
    daemonize(cmd);

    /**
     *确保只有一个守护进程实例
     */
    if (already_running()) {
        syslog(LOG_ERR, "daemon already running");
        exit(1);
    }

    /**
     * 注册我们感兴趣的信号处理函数，这里我们注册SIGHUP信号和SIGTERM信号
     * 当SIGHUP信号递达时执行重新读取配置文件操作，在执行SIGHUP信号处理函数期间需要临时阻塞SIGTERM信号
     * 当SIGTERM信号递达时终止进程，在执行SIGTERM信号处理函数期间需要临时阻塞SIGHUP信号
     */
    sa.sa_handler = sig_hup;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGTERM);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        syslog(LOG_ERR, "can't catch SIGHUP：%s", strerror(errno));
        exit(1);
    }
    sa.sa_handler = sig_term;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGHUP);
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, NULL) < 0) {
        syslog(LOG_ERR, "can't catch SIGHUP：%s", strerror(errno));
        exit(1);
    }

    /**
     * 守护进程其它逻辑代码
     */
    while (1) {
        sleep(1);
    }

    exit(0);
}


/*
实验：

[dendi875@localhost Chapter-13-Daemon Processes]$ ./reread2
[dendi875@localhost Chapter-13-Daemon Processes]$ tail -f /var/log/messages
tail: 无法打开"/var/log/messages" 读取数据: 权限不够
[dendi875@localhost Chapter-13-Daemon Processes]$ sudo tail -f /var/log/messages
[sudo] password for dendi875:
Apr 26 15:48:44 localhost dhclient[1998]: DHCPREQUEST on eth0 to 192.168.100.254 port 67 (xid=0x507ae328)
Apr 26 15:48:44 localhost dhclient[1998]: DHCPACK from 192.168.100.254 (xid=0x507ae328)
Apr 26 15:48:44 localhost NetworkManager[1928]: <info> (eth0): DHCPv4 state changed renew -> renew
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   address 192.168.100.142
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   prefix 24 (255.255.255.0)
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   gateway 192.168.100.2
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   nameserver '192.168.100.2'
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   domain name 'localdomain'
Apr 26 15:48:44 localhost dhclient[1998]: bound to 192.168.100.142 -- renewal in 829 seconds.
Apr 26 15:58:00 localhost reread2: can't open /var/run/daemon.pid：Permission denied


[root@localhost Chapter-13-Daemon Processes]# ps aux | grep reread
root     16437  0.0  0.0   1868   284 ?        S    15:59   0:00 ./reread2
root     16440  0.0  0.0   5976   752 pts/0    S+   15:59   0:00 grep --color=auto reread
[root@localhost Chapter-13-Daemon Processes]# ps -Lwf 16437
UID        PID  PPID   LWP  C NLWP STIME TTY      STAT   TIME CMD
root     16437     1 16437  0    1 15:59 ?        S      0:00 ./reread2


发送SIGHUP信号
[root@localhost Chapter-13-Daemon Processes]# kill -SIGHUP 16437
查看进程还存不存在
[root@localhost Chapter-13-Daemon Processes]# ps aux | grep reread
root     16437  0.0  0.0   2000   496 ?        S    15:59   0:00 ./reread2
root     16454  0.0  0.0   5976   752 pts/0    S+   16:00   0:00 grep --color=auto reread

另一个窗口查看日志
[dendi875@localhost Chapter-13-Daemon Processes]$ sudo tail -f /var/log/messages
[sudo] password for dendi875:
Apr 26 15:48:44 localhost dhclient[1998]: DHCPREQUEST on eth0 to 192.168.100.254 port 67 (xid=0x507ae328)
Apr 26 15:48:44 localhost dhclient[1998]: DHCPACK from 192.168.100.254 (xid=0x507ae328)
Apr 26 15:48:44 localhost NetworkManager[1928]: <info> (eth0): DHCPv4 state changed renew -> renew
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   address 192.168.100.142
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   prefix 24 (255.255.255.0)
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   gateway 192.168.100.2
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   nameserver '192.168.100.2'
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   domain name 'localdomain'
Apr 26 15:48:44 localhost dhclient[1998]: bound to 192.168.100.142 -- renewal in 829 seconds.
Apr 26 15:58:00 localhost reread2: can't open /var/run/daemon.pid：Permission denied
Apr 26 16:00:34 localhost reread2: Re-reading configuration file


发送SIGTERM信号
[root@localhost Chapter-13-Daemon Processes]# kill -SIGTERM 16437
查看进程还存不存在
[root@localhost Chapter-13-Daemon Processes]# ps aux | grep reread
root     16473  0.0  0.0   5976   752 pts/0    S+   16:01   0:00 grep --color=auto reread

另一个窗口查看日志
[dendi875@localhost Chapter-13-Daemon Processes]$ sudo tail -f /var/log/messages
[sudo] password for dendi875:
Apr 26 15:48:44 localhost dhclient[1998]: DHCPREQUEST on eth0 to 192.168.100.254 port 67 (xid=0x507ae328)
Apr 26 15:48:44 localhost dhclient[1998]: DHCPACK from 192.168.100.254 (xid=0x507ae328)
Apr 26 15:48:44 localhost NetworkManager[1928]: <info> (eth0): DHCPv4 state changed renew -> renew
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   address 192.168.100.142
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   prefix 24 (255.255.255.0)
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   gateway 192.168.100.2
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   nameserver '192.168.100.2'
Apr 26 15:48:44 localhost NetworkManager[1928]: <info>   domain name 'localdomain'
Apr 26 15:48:44 localhost dhclient[1998]: bound to 192.168.100.142 -- renewal in 829 seconds.
Apr 26 15:58:00 localhost reread2: can't open /var/run/daemon.pid：Permission denied
Apr 26 16:00:34 localhost reread2: Re-reading configuration file
Apr 26 16:01:41 localhost reread2: got SIGTERM; exiting

*/