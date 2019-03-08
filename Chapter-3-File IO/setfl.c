/**
 * 对一个文件描述符设置一个或多个文件状态标志
 */
#include "apue.h"
#include <fcntl.h>

#define BUFFSIZE    4096

static void set_fl(int fd, int flags);

int main(void)
{
    set_fl(STDOUT_FILENO, O_SYNC);

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

void set_fl(int fd, int flags)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("fcntl F_GETFL error");
    }

    val |= flags;

    if (fcntl(fd, F_SETFL, val) < 0) {
        err_sys("fcntl F_SETFL error");
    }
}

/**
 * 在修改文件描述符标志或状态标志时，必须首先获取现有的标志值，然后再按期望修改它，
 * 最后设置新标志值。不能只执行F_SETFD或F_SETFL命令，这样会关闭以前设置的标志位。
 *
 * [dendi875@localhost Chapter-3-File IO]$ ./setfl
 * 12345
 * 12345
 */