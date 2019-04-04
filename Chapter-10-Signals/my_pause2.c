/**
 * pause函数实例，如果注册的捕捉函数是SIG_IGN，pause也不会返回
 */

#include "apue.h"
#include <signal.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
    if (signal(SIGINT, SIG_IGN) == SIG_ERR) {
        err_sys("signal(SIGINT) error");
    }

    pause();
    if (errno == EINTR) {
        printf("pause returned errno is set to EINTR\n");
    }

    exit(0);
}