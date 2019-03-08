/**
 *strerror和perror的用法示例
 */
#include "apue.h"
#include <errno.h>

int main(int argc, char **argv)
{
    fprintf(stderr, "EACCESS：%s\n", strerror(EACCES));
    errno = ENOENT;
    perror(argv[0]);

    exit(0);
}

/**
 * man 3 errno　查看所有的错误常量
 *
 * 实验：
 * [dendi875@localhost Chapter-1-System Overview]$ ./testerror
 * EACCESS：Permission denied
 * ./testerror: No such file or directory
 */