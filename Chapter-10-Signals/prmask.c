/**
 * 打印当前进程阻塞信号集中的信号（也就是阻塞信号集中所在位值为1的信号）
 */

#include "apue.h"
#include <errno.h>

void pr_mask(const char *str);

int main(void)
{
    pr_mask("starting main: ");

    exit(0);
}

void pr_mask(const char *str)
{
    sigset_t    sigset;
    int         errno_save;

    /**
     * 因为信号处理函数有可能会改变errno的值所以这里先保存起来，等调用完成后再恢复
     */
    errno_save = errno;
    if (sigprocmask(0, NULL, &sigset) < 0) {
        err_ret("sigprocmask error");
    } else {
        printf("%s", str);
        if (sigismember(&sigset, SIGHUP)) {
            printf(" SIGHUP");
        }
        if (sigismember(&sigset, SIGINT)) {
            printf(" SIGINT");
        }
        if (sigismember(&sigset, SIGQUIT)) {
            printf(" SIGQUIT");
        }
        if (sigismember(&sigset, SIGABRT)) {
            printf(" SIGABRT");
        }
        if (sigismember(&sigset, SIGFPE)) {
            printf(" SIGFPE");
        }
        if (sigismember(&sigset, SIGKILL)) {
            printf(" SIGKILL");
        }
        if (sigismember(&sigset, SIGUSR1)) {
            printf(" SIGUSR1");
        }
        if (sigismember(&sigset, SIGSEGV)) {
            printf(" SIGSEGV");
        }
        if (sigismember(&sigset, SIGALRM)) {
            printf(" SIGALRM");
        }
        if (sigismember(&sigset, SIGTERM)) {
            printf(" SIGTERM");
        }
        if (sigismember(&sigset, SIGCHLD)) {
            printf(" SIGCHLD");
        }
        if (sigismember(&sigset, SIGSTOP)) {
            printf(" SIGSTOP");
        }
        if (sigismember(&sigset, SIGTTIN)) {
            printf(" SIGTTIN");
        }
        if (sigismember(&sigset, SIGTTOU)) {
            printf(" SIGTTOU");
        }
        printf("\n");
    }

    errno = errno_save;
}