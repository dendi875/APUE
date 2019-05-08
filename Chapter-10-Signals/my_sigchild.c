/**
 *
 */

#include "apue.h"
#include <sys/wait.h>

void pr_exit(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termination, signal number = %d%s\n",
            WTERMSIG(status),
#ifdef  WCOREDUMP
            WCOREDUMP(status) ? "（core file generated）" : "");
#else
        "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stoped, signal number = %d\n", WSTOPSIG(status));
    }
}

void sig_chld(int signo)
{
    pid_t    pid;
    int      status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        pr_exit(status);
    }

/*    if ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        pr_exit(status);
    }*/
}

int main(void)
{

    pid_t       pid;
    int         i;

    for (i = 0; i < 10; i++) {
        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) {  /* 子进程中 */
            break;
        }
    }

    if (pid == 0) {     /* 10个子进程 */
        int n = 1;
        while (n--) {
            printf("child ID %d\n", getpid());
            sleep(1);
        }
        exit(i+1);
    } else if (pid > 0) {   /* 一个父进程 */
        // 阻塞SIGCHLD
        struct sigaction    act;

        act.sa_handler = sig_chld;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;

        if (sigaction(SIGCHLD, &act, NULL)) {
            err_sys("sigaction(SIGCHLD) error");
        }
        // 解除对SIGCHLD的阻塞
        while (1) {
            printf("parent ID %d\n", getpid());
            sleep(1);
        }
    }

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_sigchild
parent ID 4535
child ID 4539
child ID 4540
child ID 4541
child ID 4542
child ID 4543
child ID 4544
child ID 4545
child ID 4538
child ID 4537
child ID 4536
parent ID 4535
normal termination, exit status = 4
parent ID 4535
normal termination, exit status = 5
parent ID 4535
normal termination, exit status = 6
parent ID 4535
normal termination, exit status = 7
parent ID 4535
normal termination, exit status = 8
parent ID 4535
normal termination, exit status = 9
parent ID 4535
normal termination, exit status = 10
parent ID 4535
normal termination, exit status = 3
parent ID 4535
normal termination, exit status = 2
parent ID 4535
normal termination, exit status = 1
parent ID 4535
parent ID 4535
parent ID 4535
parent ID 4535
parent ID 4535
parent ID 4535
^C

1、如果把30行的while换成if那么有可能会出问题，虽然每个子进程结束后都会发送SIGCHLD信号，
但如果某个进程（比如3号进程）正在处理SIGCHLD信号时，其他子进程（比如5号、7号）刚好结束，
这时因为SIGCHLD处于阻塞状态（sigaction函数执行信号处理函数时会阻塞当前信号）那么当执行
完信号处理函数时那些阻塞的不支持排队的SIGCHILD信号最多只有一个会递达。所以有可能就会出现
僵尸进程。

2、该程序还不完善，应该在61行阻塞SIGCHLD信号，在71号解除SIGCHLD信号的阻塞。防止在sigaction
函数调用成功之前有子进程退出。
*/