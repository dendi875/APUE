/**
 *abort函数实例；
 *abort函数并不会理会进程对SIGABRT信号的忽略，就算程序设置对该信号的忽略abort仍然会终止进程
 */

#include "apue.h"
#include <stdlib.h>
#include <signal.h>

int main(void)
{
	struct sigaction act;

	act.sa_handler = SIG_IGN;
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

/*[dendi875@localhost Chapter-10-Signals]$ ./my_abort
process starting
已放弃*/