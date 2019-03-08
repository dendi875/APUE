/**
 * 将标准输入复制到标准输出
 */
#include "apue.h"

#define BUFFSIZE    4096

int main(int argc, char **argv)
{
    int n;
    char buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            err_sys("wirte error");
        }
    }

    if (n < 0) {
        err_sys("read error");
    }

    exit(0);
}

/**
 * 实验
 * [dendi875@localhost Chapter-3-File IO]$ ./copy
 * 12345
 * 12345
 *
 * 按ctrl+d结束
 */