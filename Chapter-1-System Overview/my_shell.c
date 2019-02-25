#include "apue.h"
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t pid;
    int status;
    char buf[MAXLINE];

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = 0;
        }

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) {
            execlp(buf, buf, (char *)NULL);
            err_ret("couldn't execute：%s", buf);
            exit(127);
        }

        // 父进程执行逻辑
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            err_sys("waitpid error");
        }
        printf("%% ");
    }

    exit(0);
}