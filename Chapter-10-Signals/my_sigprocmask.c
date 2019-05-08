/**
 *sigprocmask函数实例
 */

#include "apue.h"
#include <signal.h>

static void pr_sigset(sigset_t *);
static void sig_handler(int);

int main(void)
{
	sigset_t newset, pendset;

	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		err_sys("can't catch SIGINT");
	}
	if (signal(SIGQUIT, sig_handler) == SIG_ERR) {
		err_sys("can't catch SIGQUIT");
	}

	sigemptyset(&newset);	/* 清空信号集中的信号 */
	sigaddset(&newset, SIGINT);	/* 把SIGINT信号加到信号集中 */

	/* 把SIGINT信号加入到阻塞信号集中 */
	if (sigprocmask(SIG_BLOCK, &newset, NULL) < 0) {
		err_sys("SIG_BLOCK error");
	}

	for (;;) {
		if (sigpending(&pendset) < 0) {
		    err_sys("sigpending error");
		}
		pr_sigset(&pendset);
		sleep(1);
	}

	exit(0);
}

static void pr_sigset(sigset_t *set)
{
    int i;

    for (i = 1; i < _NSIG; i++) {
        if (sigismember(set, i)) {
            fputc('1', stdout);
        } else {
            fputc('0', stdout);
        }
    }
    putchar('\n');
}

static void sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("received a signo = %d\n", signo);
    } else if (signo == SIGQUIT) {
        sigset_t newset;	/* 构造一个信号集 */
        sigemptyset(&newset);	/* 清空信号集中的所有信号 */
        sigaddset(&newset, SIGINT);	/* 把SIGINT添加到信号集中 */
        /* 从当前进程的阻塞信号集中解除对SIGNIT信号的阻塞 */
        if (sigprocmask(SIG_UNBLOCK, &newset, NULL) < 0) {
        	err_sys("SIG_UNBLOCK error");
        }
    }
}

/*
实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_sigprocmask
0000000000000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000000000000
^C0100000000000000000000000000000000000000000000000000000000000000
0100000000000000000000000000000000000000000000000000000000000000
0100000000000000000000000000000000000000000000000000000000000000
^\received a signo = 2
0000000000000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000000000000
^\退出 (core dumped)

为什么第二次ctrl+\时执行的是系统默认的处理动作，还没有执行信号处理函数sig_handler？


1、sigprocmask函数返回之前会查看未决信号集，如果未决信号集中有未决的信号则至少把一个信号
递送给进程，也就是进程需要执行完未决信号对应的信号处理函数后sigprocmask才返回继续执行后面的代码
*/