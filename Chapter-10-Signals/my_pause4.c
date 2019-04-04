/**
 * pause函数实例
 */

#include "apue.h"
#include <signal.h>
#include <errno.h>
#include <unistd.h>

static void sig_int(int);

int main(void)
{
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        err_sys("signal(SIGINT) error");
    }

    pause();
    if (errno == EINTR) {
        printf("pause returned, errno is set to EINTR\n");
    }

    exit(0);
}

static void sig_int(int signo)
{
    /* 什么都不做，仅为了使pause返回 */
}

/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_pause4
^Cpause returned, errno is set to EINTR*/