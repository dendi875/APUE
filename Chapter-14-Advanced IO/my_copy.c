/**
 * 一个阻塞I/0的实例，从标准输入读取数据复制到标准输出
 */

#include "apue.h"

#define BUFFSIZE 5

int main(void)
{
    char    buf[BUFFSIZE];
    int     n;

    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            err_sys("write error");
        }
    }
    if (n < 0) {    /* 等于0表示读到了文件末尾 */
        err_sys("read error");
    }

    exit(0);
}