/**
 * 使用 popen 执行 ./myuclc 程序。
 *
 * 因为子进程的标准输出被重定向指向了管道的写端，所以 执行./myucls调用putchar时是向管道的写端写入数据，
 * 父进程是从读管道文件描述符对应的文件指针中读数据。
 *
 * 注意与 popen.c 程序做对比
 */

#include "apue.h"

int main(void)
{
    char line[MAXLINE];
    FILE *fpin;

    if ((fpin = popen("./myuclc", "r")) == NULL) {
        err_sys("popen error");
    }

    for (;;) {
        fputs("prompt>", stdout);   /* 提示 */
        fflush(stdout);
        if (fgets(line, MAXLINE, fpin) == NULL) {   /* 父进程从管道中读数据 */
            break;
        }
        if (fputs(line, stdout) == EOF) {
            err_sys("fputs error to pipe");
        }
    }

    if (pclose(fpin) == -1) {
        err_sys("pclose error");
    }
    putchar('\n');

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-15-Interprocess Communication]$ ./popen2
prompt>ABcdEF
abcdef
prompt>AAAAA
aaaaa
prompt>
*./