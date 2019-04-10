/**
 * 孤儿进程组实例
 */

#include "apue.h"
#include <errno.h>

static void pr_ids(char *);
static void sig_hup(int);

int main(void)
{
    char    c;
    pid_t   pid;

    pr_ids("parent");           /* 先打印父进程的各种id */
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {       /* 父进程中 */
        sleep(5);
    } else {                    /* 子进程中 */
        pr_ids("child");
        signal(SIGHUP, sig_hup);    /* 注册一个信号处理器 */
        kill(getpid(), SIGTSTP);    /* 子进程暂停自己（相当于ctrl+z）*/
        pr_ids("child");            /* 当我们继续运行再次打印 */
        if (read(STDIN_FILENO, &c, 1) != 1) {
            printf("read error %d on controlling TTY\n", errno);
        }
    }
    exit(0);
}

static void pr_ids(char *name)
{
    printf("%s：pid = %ld, ppid = %ld, gpid = %ld, sid = %ld, tpgid = %ld\n",
            name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)getsid(0), (long)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}

static void sig_hup(int signo)
{
    printf("SIGHUP received, pid = %ld\n", (long)getpid());
}



/*
孤儿进程组：进程组中的每个进程的父进程都属于另一个会话。

一个进程组不是孤儿进程组的条件是：
该组中有一个进程，其父进程在属于同一会话的另一个组中。
如果进程组不是孤儿进程组，那么在属于同一会话的另一个组中的父进程就有机会重新启动该组中停止的进程。

1、父进程sleep(5)，子进程先运行
2、子进程注册SIGHUP信号处理程序
3、子进程用kill函数向自已发送SIGTSTP信号（相当于ctrl+z停止一个前台作业）。
4、父进程开始运行，当父进程终止后，子进程成为孤儿进程，子进程的父进程为init进程
5、在父进程终止后，进程组包含一个停止的进程，进程组为孤儿进程组，这时内核就会向孤儿进程组中处于停止
状态的每一个进程发送挂断信号（SIGHUP），接口又向其发送继续信号（SIGCONT）
6、在子进程调用pr_ids后，子进程是后台进程组，当后台进程组试图读控制终止时，对该后台进程组产生SIGTTIN
信号，但在这里，这是一个孤儿进程组，如果内核用SIGTTIN停止它，则些进程组中的进程就再也不会继续。
POSIX规定，read返回出错，其errno设置为EIO（值为5）


父进程终止时，子进程变成后台进程组，这是因为父进程是由shell作为前台作业执行的，一个会话只有一个前台
进程组，其它都是后台进程组


实验：
[dendi875@localhost Chapter-9-Process Relationships]$ ./orphan
parent：pid = 21161, ppid = 17600, gpid = 21161, sid = 17600, tpgid = 21161
child：pid = 21162, ppid = 21161, gpid = 21161, sid = 17600, tpgid = 21161
[dendi875@localhost Chapter-9-Process Relationships]$ SIGHUP received, pid = 21162
child：pid = 21162, ppid = 1, gpid = 21161, sid = 17600, tpgid = 17600
read error 5 on controlling TTY
*/