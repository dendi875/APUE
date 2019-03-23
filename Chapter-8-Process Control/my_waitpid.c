/**
 * 使用waitpid非阻塞回收指定的子进程实例，与my_wait.c阻塞版本做对比
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
    } else if (pid == 0) {      /* 子进程执行代码 */
        printf("I am child process pid：%d\tppid：%d\n", getpid(), getppid());
        sleep(30);
        printf("child process is exited.\n");
        exit(0);
    }

    while (1) {                /* 父进程执行代码 */
        printf("I am parent process pid：%d\tppid：%d\n", getpid(), getppid());
        w = waitpid(pid, &status, WNOHANG);
        // w = waitpid(pid, &status, 0);
        if (w == -1) {
            err_sys("waitpid error");
        } else if (w == pid) {
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
