/**
 *用此程序来验证：当调用一个信号处理程序时，被捕捉的信号被加到当前进程的阻塞信号集中。当从信号处理
 *程序返回时，恢复原来的屏蔽字。
 */

#include "apue.h"
#include <signal.h>
#include <errno.h>

static void sig_usr1(int);
void pr_mask(const char *str);

int main(void)
{
    struct sigaction act;

    act.sa_handler = sig_usr1;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        err_sys("sigaction(SIGUSR1) error");
    }

    pr_mask("starting main：");
    pause();
    pr_mask("ending main：");

    exit(0);
}

static void sig_usr1(int signo)
{
    pr_mask("in sig_usr1：");
}

void pr_mask(const char *str)
{
    sigset_t    sigset;
    int         errno_save;

    /* 因为信号相关函数可以会改变errno的值所以这里先保存起来，等调用完成后再恢复 */
    errno_save = errno;
    if (sigprocmask(0, NULL, &sigset) < 0) {
        err_sys("sigrocmask error");
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
        if (sigismember(&sigset, SIGALRM)) {
            printf(" SIGALRM");
        }
        if (sigismember(&sigset, SIGUSR1)) {
            printf(" SIGUSR1");
        }
        fputc('\n', stdout);
    }

    errno = errno_save;
}

/*实验：
运行程序
[dendi875@localhost Chapter-10-Signals]$ ./my_sigusr1
starting main：

开另一个终端
[dendi875@localhost ~]$ ps ajxf | grep my_sigusr1
2829 19119 19119  2829 pts/0    19119 S+     504   0:00          |   \_ ./my_sigusr1

发送SIGUSR1信号给my_sigusr1程序运行的进程
[dendi875@localhost ~]$ kill -SIGUSR1 19119

查看前一个终端的输出
[dendi875@localhost Chapter-10-Signals]$ ./my_sigusr1
starting main：
in sig_usr1： SIGUSR1
ending main：*/