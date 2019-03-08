/**
 * 将标准输入复制到标准输出
 */
#include "apue.h"

#define  BUFFSIZE   4096

int main(int argc, char **argv)
{
    int n;
    char buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            err_sys("write error");
        }
    }

    if (n < 0) {
        err_sys("read error");
    }

    exit(0);
}

/**
 * 如果从终端或网络上读写数据那么就是阻塞I/O，从文件中读写数据是非阻塞I/O
 *
 * 实验：
 *
 * [dendi875@localhost Chapter-1-System Overview]$ ./copy < infile > outfile
 */