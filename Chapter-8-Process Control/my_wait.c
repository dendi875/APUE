/**
 * 使用wait阻塞回收子进程实例
 */

#include "apue.h"
#include <sys/wait.h>

void pr_exit(int);

int main(void)
{
    pid_t pid, w;
    int status;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 子进程执行的代码 */
        printf("I am child process pid：%d\tppid：%d\n", getpid(), getppid());
        sleep(30);
        printf("child process is exited.\n");
        exit(0);
    }
    while (1) {                /* 父进程执行的代码 */
        printf("I am parent process pid：%d\tppid：%d\n", getpid(), getppid());
        w = wait(&status);
        if (w == pid) {
            pr_exit(status);
        } else {
            printf("wait for child w = %d\n", w);
        }
        sleep(2);
    }
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
[dendi875@localhost Chapter-8-Process Control]$ ./my_wait
I am parent process pid：6122   ppid：3518
I am child process pid：6123    ppid：6122
child process is exited.
normal termination, exit status = 0
I am parent process pid：6122   ppid：3518
wait for child w = -1
I am parent process pid：6122   ppid：3518
wait for child w = -1
I am parent process pid：6122   ppid：3518
wait for child w = -1
I am parent process pid：6122   ppid：3518
wait for child w = -1
^C

在子进程运行期间（sleep(30)能证明子进程在运行）父进程调用wait阻塞住了，所以wait后面的printf没有输出，
等子进程结束后父进程wait对僵尸进程进行清理，返回子进程的终止进程ID，并且把终止状态设置到status变量中。
下次再调用wait时父进程下面没有子进程了这时wait返回-1*/