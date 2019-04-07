/**
 * 该程序演示了如何处理SIGTSTP信号，该程序只是将其标准输入复制到其标准输出
 */

#include "apue.h"
#include <signal.h>

#define BUFFSIZE	1024

static sig_tstp(int);
Sigfunc *my_signal(int, Sigfunc *);

int main(void)
{
	int 	n;
	char 	buf[BUFFSIZE];

	if (my_signal(SIGTSTP, SIG_IGN) == SIG_DFL) {
		my_signal(SIGTSTP, sig_tstp);
	}

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			err_sys("write error");
		}
	}

	if (n < 0) {
		err_sys("read error");
	}

	exit(0);
}

static sig_tstp(int signo)
{
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGTSTP);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	my_signal(SIGTSTP, SIG_DFL);

	kill(getpid(), SIGTSTP);

	my_signal(SIGTSTP, sig_tstp);
}

Sigfunc *my_signal(int signo, Sigfunc *func)
{
	struct sigaction act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
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
