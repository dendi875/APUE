/**
 *父进程使用wait获取子进程不同终止方式的状态
 */

#include "apue.h"
#include <sys/wait.h>

void pr_exit(int);

int main(void)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 子进程中 */
        exit(7);
    }
    /**
     * 父进程等待获取子进程终止状态
     * 如果父进程下没有子进程了，那么返回-1
     * 如果父进程下的子进程都在运行，那么阻塞住
     * 如果父进程下刚好有个子进程终止，这时父进程获取子进程终止状态放到status变量中，然后返回那个
     * 终止的子进程ID号
     */
    if (wait(&status) != pid) {
        err_sys("wait error");
    }
    pr_exit(status);

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {     /* 子进程中 */
        abort();               /* 生成SIGABRT信号 */
    }
    if (wait(&status) != pid) {
        err_sys("wait error");
    }
    pr_exit(status);

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 子进程中 */
        status /= 0;            /* status除以0生成SIGFPE信号，FPE：floating-point exception（浮点异常）*/
    }
    if (wait(&status) != pid) {
        err_sys("wait error");
    }
    pr_exit(status);

    exit(0);

}

void pr_exit(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termination, signal number = %d%s\n",
            WTERMSIG(status),
#ifdef WCOREDUMP
            WCOREDUMP(status) ? "（core file generated）" : "");
#else
            "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stoped, signal number = %dn", WSTOPSIG(status));
    }
}


/*实验：
运行程序
[dendi875@localhost Chapter-8-Process Control]$ ./wait1
normal termination, exit status = 7
abnormal termination, signal number = 6
abnormal termination, signal number = 8

使用kill -l查看信号，SIGABRT编号为6，SIGFPE为8
[dendi875@localhost Chapter-8-Process Control]$ kill -l
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL       5) SIGTRAP
 6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1*/