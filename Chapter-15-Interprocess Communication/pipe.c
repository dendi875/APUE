/**
 * 父进程通过管道向子进程传递数据
 */

#include "apue.h"

int main(void)
{
    int fd[2];
    pid_t pid;
    int n;
    char line[MAXLINE];

    if (pipe(fd) < 0) {
        err_sys("pipe error");
    }
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {   /* 父进程 */
        close(fd[0]);
        write(fd[1], "hello world\n", 12);
    } else {    /* 子进程 */
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }

    exit(0);
}