/**
 * 先创建一个管道，fork 一个子进程，使子进程的标准输入指向管道的读端，然后执行 execl ，执行分页程序
 */

#include "apue.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DEF_PAGER  "/bin/more"  /* 默认的分页程序 */

int main(int argc, char *argv[])
{
    int     fd[2];
    pid_t   pid;
    FILE    *fp;
    char    line[MAXLINE];
    int     n;
    char    *pager, *argv0;

    if (argc != 2) {
        err_quit("usage：./pipe2 <pathname>");
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        err_sys("can't open %s", argv[1]);
    }

    if (pipe(fd) < 0) {
        err_sys("pipe error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {   /* 父进程 */
        close(fd[0]);

        /* 父进程将 argv[1] 复制到管道 */
        while (fgets(line, MAXLINE, fp) != NULL) {
            n = strlen(line);
            if (write(fd[1], line, n) != n) {
                err_sys("write error to pipe");
            }
        }

        if (ferror(fp)) {
            err_sys("fgets error");
        }

        close(fd[1]);

        if (waitpid(pid, NULL, 0) < 0) {
            err_sys("waitpid error");
        }

        exit(0);
    } else {        /* 子进程 */
        close(fd[1]);

        /**
         * 使标准输入重定向到管道的读端
         */
        if (fd[0] != STDIN_FILENO) {
            if (dup2(fd[0], STDIN_FILENO) < 0) {
                err_sys("dup2 error to stdin");
            }
            close(fd[0]);
        }

        if ((pager = getenv("PAGER")) == NULL) {
            pager = DEF_PAGER;
        }

        if ((argv0 = strrchr(pager, '/')) != NULL) {
            argv0++;
        } else {
            argv0 = pager;
        }

        if (execl(pager, argv0, (char *)0) < 0) {
            err_sys("execl error for %s", pager);
        }
    }

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-15-Interprocess Communication]$ ./pipe2  /etc/services
*/