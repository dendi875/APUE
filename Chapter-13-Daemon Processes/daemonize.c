/**
 * 初始化守护进程实例
 */

#include "apue.h"
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>

void daemonize(const char *cmd)
{
    int                 i, fd0, fd1, fd2;
    pid_t               pid;
    struct              rlimit rlim;
    struct  sigaction   act;
    /**
     * 重新设置文件权限掩码
     */
    umask(0);

    /**
     * 获取进程最大的文件描述符
     */
    if (getrlimit(RLIMIT_NOFILE, &rlim)) {
        err_quit("%s： can't get file limit", cmd);
    }

    /**
     *成为会话的首进程并丢弃控制终端
     */
    if ((pid = fork()) < 0) {
        err_sys("%s：fork error", cmd);
    } else if (pid > 0) {       /* 父进程 */
        exit(0);
    }
    setsid();

    /**
     * 1、忽略SIGHUP信号
     * 2、基于System V的系统中，再次进行fork，终止父进程，继续使用子进程中的守护进程。
     * 避免将来再次调用open且没有指定O_NOCTTY时会获取控制终端
     */
     act.sa_handler = SIG_IGN;
     sigemptyset(&act.sa_mask);
     act.sa_flags = 0;
     if (sigaction(SIGHUP, &act, NULL) < 0) {
        err_sys("%s：can't ignore SIGHUP", cmd);
     }
     if ((pid = fork()) < 0) {
        err_quit("%s：can't fork", cmd);
     } else if (pid > 0) {     /* 父进程 */
        exit(0);
     }

     /**
      * 改变当前工作目录
      */
    if (chdir("/") < 0) {
        err_quit("%s：can't change directory to /", cmd);
    }
    if (rlim.rlim_max == RLIM_INFINITY) {
        rlim.rlim_max = 1024;
    }
    for (i = 0; i < rlim.rlim_max; i++) {
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

int main(void)
{
    daemonize("testdaemon");

    while (1) {
        sleep(1);
    }

    exit(0);
}


/**
 *　创建守护进程步骤：
 *　1、重新设置文件权限掩码
 *　umask 函数，防止使用继承过来的掩码来创建文件可能会被设置为拒绝某些权限
 *　2、调用fork创建子进程，父进程退出
 *　fork  函数，保证了子进程不是一个组长进程，这是执行setsid调用的先决条件
 *　3、调用setsid创建一个新的会话
 *　setsid 函数，目的是使调用进程：
 *　    a）成为新会话的首进程
 *　    b）成为一个新进程组的组长进程
 *　    c）没有控制终端
 *　4、更改当前工作目录，一般为根目录
 *　chdir 函数，防止占用可卸载的文件系统
 *　5、关闭所有的文件描述符
 *　    getrlimit、close函数，从父进程继承过来的文件描述符不会再被用到，如果不关闭就浪费了系统资源
 *　6、使用0、1、2文件描述符指向/dev/null
 *　open、dup函数，目的是使任何一个试图从标准输入读、写到标准输出、写到标准错误的程序都不会产生效果，
 *　因为守护进程并不与终端设备相关联，所以其输出无处显示，也无处从交互式用户那里接收输入。
 */