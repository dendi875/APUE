/**
 * sigsuspend函数的一个应用情景：
 * 等待一个信号处理函数设置一个全局变量
 */

#include "apue.h"
#include <signal.h>

volatile sig_atomic_t	quitflag;	/* 默认值为0 */

static void sig_int(int);

int main(void)
{
	sigset_t			newmask, oldmask, zeromask;
	struct sigaction 	int_act, quit_act;

	int_act.sa_handler = sig_int;
	sigemptyset(&int_act.sa_mask);
	int_act.sa_flags = 0;

	quit_act.sa_handler = sig_int;
	sigemptyset(&quit_act.sa_mask);
	quit_act.sa_flags = 0;
	if (sigaction(SIGINT, &int_act, NULL) < 0) {
		err_sys("sigaction(SIGINT) error");
	}
	if (sigaction(SIGQUIT, &quit_act, NULL) < 0) {
		err_sys("sigaction(SIGQUIT) error");
	}

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);
	sigemptyset(&zeromask);

	/**
	 * 把SIGINI信号加入当前进程的阻塞信号集，并保存还没有加入之前的阻塞信号集的状态
	 */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		err_sys("SIG_BLOCK error");
	}

	/**
	 * 处进程挂起等待信号处理函数改变全局变量的值
	 */
	while (quitflag == 0) {
		sigsuspend(&zeromask);
	}

	/**
	 * SIGQUIT信号已经被捕捉，sigsuspend函数也已经从信号处理函数中返回并把进程的阻塞信号集恢复
	 * 成调用sigsuspend之前的状态
	 */
	quitflag = 0;

	/**
	 * 重置进程的阻塞信号集，解除对SIGQUIT信号的阻塞
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		err_sys("SIG_SETMASK error");
	}

	exit(0);

}

/**
 * 信号处理函数，处理SININT和SIGQUIT信号
 */
static void sig_int(int signo)
{
	if (signo == SIGINT) {
		printf("\ninterrupt\n");
	} else if (signo == SIGQUIT) {
		quitflag = 1;
	}
}


/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./sigsuspend2
^C
interrupt
^C
interrupt
^C
interrupt
^\[dendi875@localhost Chapter-10-Signals]$ */