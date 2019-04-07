/**
 * sigaction函数实例
 */

#include "apue.h"
#include <signal.h>

static void sig_int(int);

int main(void)
{
	struct sigaction act;

	act.sa_handler = sig_int;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;	/* 使用sa_handler的方式调用信号处理程序 */
#ifdef	SA_INTERRUPT
	act.sa_flags |= SA_INTERRUPT;	/* 所有信号导致的系统调用中断后，都不自动重启 */
#endif

	if (sigaction(SIGINT, &act, NULL) < 0) {
		err_sys("sigaction(SIGINT) error");
	}

	for (;;) {
		printf("*********\n");
		sleep(1);
	}

	return 0;
}

static void sig_int(int signo)
{
	printf("in sig_int signo = %d\n", signo);
}

/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_sigaction
*********
*********
^Cin sig_int signo = 2
*********
*********
^Cin sig_int signo = 2
*********
^\退出 (core dumped)*/