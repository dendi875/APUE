/**
 * pause函数实例，说明注册的信号处理程序是SIG_DFL时pause也不会返回
 */

#include "apue.h"
#include <signal.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
    if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
        err_sys("signal(SIGINT) error");
    }

    pause();

    if (errno == EINTR) {
        printf("pause returned, and errno is set to EINTR\n");
    }

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_pause3
^C*/