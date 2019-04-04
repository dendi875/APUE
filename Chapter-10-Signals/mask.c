/**
 *该程序说明：当调用一个信号处理程序时，被捕捉的信号会被自动加入到当前进程的阻塞信号集中
 *（阻塞信号集中该信号所对应位的值自动设置为1）。当从信号处理程序返回时，恢复原来的阻塞信号集
 *（阻塞信号集中该信号所对应位的值自动恢复为0）。另外，siglongjmp恢复了由sigsetjmp保存的阻塞信号集。
 */

#include "apue.h"
#include <signal.h>
#include <setjmp.h>
#include <time.h>
#include <errno.h>

static sigjmp_buf               jmpbuf;
static volatile sig_atomic_t    canjmp;

static void                     sig_usr1(int);
static void                     sig_alrm(int);
void                            pr_mask(const char *str);

int main(void)
{
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {
        err_sys("signal(SIGUSR1) error");
    }
    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        err_sys("signal(SIGALRM) error");
    }

    pr_mask("starting main：");

    if (sigsetjmp(jmpbuf, 1)) {  /* 如果sigsetjmp第二个参数是非0，则保存当前进程的阻塞信号集 */
        pr_mask("ending main：");
        exit(0);
    }

    canjmp = 1;

    /**
     * 使当前进程挂起，直到捕捉了一个信号。
     * 只有当执行了一个信号处理程序并从其返回时，pause才返回。这种情况下pause返回-1，errno设置为EINTR。
     * 系统默认的处理动作不会使pause返回，
     * 如果注册的捕捉函数是SIG_IGN，pause也不会返回
     * 如果注册的捕捉函数是SIG_DFL，pause也不会返回
     */
    for (;;) {
        pause();
    }
}

static void sig_usr1(int signo)
{
    time_t    starttime;

    /**
     *这样做目的是为了防止jmpbuf（跳转缓冲）还末被sigsetjmp初始化，就有SIGUSR1信号产生调用sig_usr1
     *函数。因为信号可能在任何时候产生。
     */
    if (canjmp == 0) {
        return;
    }

    pr_mask("starting sig_usr1：");  /* 内核会自动把SIGUSR1信号加入到该进程的阻塞信号集中 */

    alarm(3);                        /* 安装一个定时器3秒超时 */
    starttime = time(NULL);
    for (;;) {
        if (time(NULL) > starttime + 5) {
            break;
        }
    }

    pr_mask("finishing sig_usr1：");  /* 当从信号处理程序返回时SIGALRM信号从阻塞信号集中解除 */

    canjmp = 0;
    siglongjmp(jmpbuf, 1);           /* 返回到sigsetjmp处，并恢复sigsetjmp保存的阻塞信号集 */
}

static void sig_alrm(int signo)
{
    pr_mask("in sig_alrm：");    /* 内核会自动把SIGALRM信号加入到该进程的阻塞信号集中 */
}


void pr_mask(const char *str)
{
    sigset_t    sigset;
    int         errno_save;

    /* 因为信号操作相关函数会修改errno的值，所以这里先保存起来，等调用完成后再恢复 */
    errno_save = errno;
    if (sigprocmask(0, NULL, &sigset) < 0) {
        err_sys("sigprocmask error");
    } else {
        printf("%s", str);
        if (sigismember(&sigset, SIGHUP)) {
            printf(" SIGHUP");
        }
        if (sigismember(&sigset, SIGINT)) {
            printf(" SIGINT");
        }
        if (sigismember(&sigset, SIGUSR1)) {
            printf(" SIGUSR1");
        }
        if (sigismember(&sigset, SIGALRM)) {
            printf(" SIGALRM");
        }
        fputc('\n', stdout);
    }

    errno = errno_save;
}