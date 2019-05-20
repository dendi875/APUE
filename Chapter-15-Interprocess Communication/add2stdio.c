/**
 * 协同进程程序，使用 popen3.c 程序 execl 执行该程序。使用标准I/O从标准输入读取两个数，
 * 计算它们的和，然后将和使用标准I/O函数写至标准输出。
 *
 * 默认情况下标准输入/标准输出是指向交互工的终端设备（/dev/tty）它是行缓冲的。
 * 但现在 标准输入/标准输出 被重定向到了指向管道，它变成全缓冲的，就会造成死锁。
 * 所以需要用 setvbuf 改变标准输入/标准输出的缓冲方式。
 *
 */

#include "apue.h"

int main(void)
{
    int     int1, int2;
    char    line[MAXLINE];

    if (setvbuf(stdin, NULL, _IOLBF, 0) != 0) {
        err_sys("setvbuf error");
    }
    if (setvbuf(stdout, NULL, _IOLBF, 0) != 0) {
        err_sys("setvbuf error");
    }

    while (fgets(line, MAXLINE, stdin) != NULL) {
        if (sscanf(line, "%d%d", &int1, &int2) == 2) {
            if (printf("%d\n", int1 + int2) == EOF) {
                err_sys("printf error");
            }
        } else {
            if (printf("invalid args\n") == EOF) {
                err_sys("printf error");
            }
        }
    }

    exit(0);
}