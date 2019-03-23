/**
 * exec函数实例
 */

#include "apue.h"
#include <sys/wait.h>

char *env_init[] = {"USER=unknow", "PATH=/tmp", NULL};

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 子进程中执行的代码 */
        if ((execle("/home/dendi875/bin/echoall", "myarg1", "MY ARGS", (char *)0, env_init)) < 0) {
            err_sys("execle error");
        }
    }

    if (waitpid(pid, NULL, 0) < 0) {        /* 父进程阻塞等待子进程结束 */
        err_sys("wait error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {              /* 子进程中执行的代码 */
        if (execlp("echoall", "only 1 arg", (char *)0) < 0) {
            err_sys("execlp error");
        }
    }

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-8-Process Control]$ PATH=$PATH:/home/dendi875/bin
[dendi875@localhost Chapter-8-Process Control]$ ./exec1
argv[0]：myarg1
argv[1]：MY ARGS
USER=unknow
PATH=/tmp
[dendi875@localhost Chapter-8-Process Control]$ argv[0]：only 1 arg
HOSTNAME=localhost.localdomain
SHELL=/bin/bash
TERM=linux
HISTSIZE=1000
QTDIR=/usr/lib/qt-3.3
OLDPWD=/home/dendi875/bin
QTINC=/usr/lib/qt-3.3/include
....


一般情况下都是用fork创建出一个子进程来，然后用子进程exec来执行另一个程序。这也是shell的典型用法.
shell的用法：
当在命令行解释器上执行一个命令时，由shell这个进程fork出一个子进程来，然后由子进程调用exec来执行我们
给的命令。

echoall.c内容为：
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
        int             i;
        char            **ptr;
        extern char **environ;

        for (i = 0; i < argc; i++) {
                printf("argv[%d]：%s\n", i, argv[i]);
        }

        for (ptr = environ; *ptr != NULL; ptr++) {
                printf("%s\n", *ptr);
        }

        exit(0);
}*/