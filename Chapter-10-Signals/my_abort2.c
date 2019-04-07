/**
 * abort函数实例；此程序说明就算程序注册了对SIGABRT信号处理函数。当捕捉到了SIGABRT信号执行完信号
 * 处理函数后仍不会返回到其调用者那里。
 */

#include "apue.h"
#include <signal.h>

static void sig_abrt(int);

int main(void)
{
	struct sigaction act;

	act.sa_handler = sig_abrt;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	if (sigaction(SIGABRT, &act, NULL) < 0) {
		err_sys("sigaction(SIGABRT) error");
	}

	printf("process starting\n");

	abort();

	printf("process ending\n");

	exit(0);
}

static void sig_abrt(int signo)
{
	printf("in sig_abrt\n");
}

/*
实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_abort2
process starting
in sig_abrt
已放弃*/