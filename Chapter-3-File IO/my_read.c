/**
 * read()函数实例
 */
#include "apue.h"

#define BUFFSIZE 10

int main(void)
{
    int n;
    char buf[BUFFSIZE];

    if ((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) {
        err_sys("error read");
    }
    printf("n = %d\n", n);

    exit(0);
}

/**
 * ssize_t read(int fd, void *buf, size_t count);
 *
 * read()函数从fd中读取字符，存储到buf中，希望读count个字符
 * 返回值n表示实际读取到的字符。有可能n比第三个参数小（读到了末尾、遇到终止信号）
 *
 * 实验：
 * 输入123然后按回车键
 * [root@localhost Chapter-3-File IO]# ./my_read
 * 123
 * n = 4
 *
 * 输入123然后按ctrl+d（EOF）
 * [root@localhost Chapter-3-File IO]# ./my_read
 * 123n = 3
 *
 * 输入0123456789123然后按回车
 * [root@localhost Chapter-3-File IO]# ./my_read
 * 0123456789123
 * n = 10
 * [root@localhost Chapter-3-File IO]# 123
 * -bash: 123: command not found
 */