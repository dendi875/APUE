/**
 * 用标准I/O将标准输入复制到标准输出
 */
#include "apue.h"

int main(int argc, char **argv)
{
    int c;

    while ((c = getc(stdin)) != EOF) {
        if (putc(c, stdout) == EOF) {
            err_sys("output error");
        }
    }

    if (ferror(stdin)) {
        err_sys("input error");
    }

    exit(0);
}

/**
 * 实验：
 * [dendi875@localhost Chapter-1-System Overview]$ ./getcputc < infile > outfile
 */