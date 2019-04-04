/**
 * 用sigaction实现的signal函数
 */

#include "apue.h"
#include <signal.h>

Sigfunc *my_signal(int, Sigfunc *);
static void sig_int(int);

int main(void)
{
    if (my_signal(SIGINT, sig_int) == SIG_ERR) {
        err_quit("my_signal(SIGINT) error");
    }

    for (;;) {
        printf("*********\n");
        sleep(1);
    }

    return 0;
}

Sigfunc *my_signal(int signo, Sigfunc *func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
        act.sa_flags |= SA_RESTART;
    }

    if (sigaction(signo, &act, &oact) < 0) {
        return(SIG_ERR);
    }

    return(oact.sa_handler);
}

static void sig_int(int signo)
{
    printf("received signo = %d\n", signo);
}


/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_signal
*********
*********
*********
^Creceived signo = 2
*********
^Creceived signo = 2
*********
*********
^\退出 (core dumped)


SA_RESTART：由此信号导致的中断系统调用后会自动重试（again）
SA_INTERRUPT：由此信号导致的系统调用中断后不会自动重试

除了SIGALRM信号导致的中断系统调用都默认自动重试。*/