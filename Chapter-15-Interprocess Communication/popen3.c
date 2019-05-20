/**
 * 协同进程实例。
 *
 * 这个程序创建了两个管道，父进程，子进程各自关闭它们不需要的管道端。
 * 一个管道的作用是协同进程的标准输入，另一个管道的作用是协同进程的标准输出。
 * 子进程调用 dup2 使标准输入描述符指向第一个管道的读端，
 *   在次调用 dup2 使标准输出描述符指向第二个管道的写端。
 *   最后在调用 execl
 */

#include "apue.h"
#include <signal.h>

static void sig_pipe(int signo);    /* our signal handler */

int main(void)
{
    int     fd1[2], fd2[2], n;
    pid_t   pid;
    char    line[MAXLINE];
    struct  sigaction sa;

    sa.sa_handler = sig_pipe;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGPIPE, &sa, NULL) < 0) {
        err_sys("sigaction(SIGPIPE) error");
    }

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        err_sys("pipe error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {   /* 父进程中 */
        close(fd1[0]);
        close(fd2[1]);

        /**
         * 父进程从标准输入读数据然后写到第一个管道中去，接着从第二个管道中读然后写到标准输出
         */
        while (fgets(line, MAXLINE, stdin) != NULL) {
            n = strlen(line);
            if (write(fd1[1], line, n) != n) {
                err_sys("write error to pipe");
            }
            if ((n = read(fd2[0], line, MAXLINE)) < 0) {
                err_sys("read error from pipe");
            }
            if (n == 0) {
                err_msg("child closed pipe");
                break;
            }
            line[n] = 0; /* null terminate */
            if (fputs(line, stdout) == EOF) {
                err_sys("fputs error");
            }
        }

        if (ferror(stdin)) {
            err_sys("fgets error on stdin");
        }

        exit(0);
    } else {    /* 子进程中 */
        close(fd1[1]);
        close(fd2[0]);

        /**
         * 子进程重定向标准输入文件描述符，使它指向第一个管道的读端，接着，再重定向标准输出使它指向
         * 第二个管道的写端着，最后调用 execl 执行 add2.c编译后的 add2程序
         */
        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO) {
                err_sys("dup2 error top stdin");
            }
            close(fd1[0]);
        }

        if (fd2[1] != STDOUT_FILENO) {
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO) {
                err_sys("dup2 error to stdout");
            }
            close(fd2[1]);
        }

        if (execl("./add2", "add2", (char *)0) < 0) {
            err_sys("execl error");
        }


/*        if (execl("./add2stdio", "add2stdio", (char *)0) < 0) {
            err_sys("execl error");
        }*/

        exit(0);
    }
}

static void sig_pipe(int signo)
{
    printf("SIGPIPE caught\n");
    exit(1);
}

/*
实验：
[dendi875@localhost Chapter-15-Interprocess Communication]$ ./popen3
1 2
3

注意当执行 execl("./add2", "add2", (char *)0) 时 ./add2 程序时的标准输入是指向了第一个管道的读端，
标准输出是指向了第二个管道的写端。execl 文件描述符的状态是执行是默认的执行时不关闭状态
*/