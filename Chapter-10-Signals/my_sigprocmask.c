/**
 *sigprocmask函数实例
 */

#include "apue.h"
#include <signal.h>

static void sig_handler(int);

int main(void)
{

}

static void sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("received a signo = %d\n", signo);
    } else if (signo == SIGQUIT) {
        sigset_t newset;
        sigemptyset(&newset);
        sigaddset(&newset, SIGINT);
        if (sigprocmask(SIG_UNBLOCK, &newset, NULL) < 0) {

        }
    }
}