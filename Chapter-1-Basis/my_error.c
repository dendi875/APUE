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
 */