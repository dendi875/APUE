/**
 * 带有竞争条件程序实例
 */

#include "apue.h"

static void charatatime(char *);

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        charatatime("output from child\n");
    } else {
        charatatime("output from parent\n");
    }
    exit(0);
}

static void charatatime(char *str)
{
    char *ptr;
    int c;

    setbuf(stdout, NULL);   /* 关闭标准输出默认的行缓冲 */
    for (ptr = str;  (c = *ptr++) != '\0';) {
        putc(c, stdout);
    }
}


/*实验：
[dendi875@localhost Chapter-8-Process Control]$ ./tellwait1
output from parent
[dendi875@localhost Chapter-8-Process Control]$ output from child*/