/**
 * sigsuspend函数实例，保护临界区不被信号中断
 */

#include "apue.h"
#include <signal.h>
#include <errno.h>

static void			pr_mask(const char *);
static void			sig_int(int);

int main(void)
{
	sigset_t 			newmask, oldmask, waitmask;
	struct sigaction	act;

	pr_mask("program start：");

	act.sa_handler = sig_int;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	if (sigaction(SIGINT, &act, NULL) < 0) {
		err_sys("sigaction(SIGINT) error");
	}
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);
	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGUSR1);

	/**
	 *	将把SIGINI信号加入到进程的阻塞信号集中，并保存当前的阻塞信号集的状态
	 */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		err_sys("SIG_BLOCK error");
	}

	/* 代码临界区 */
	pr_mask("in critical region：");

	/**
	 * 原子性的方式阻塞SIGUSR1信号，阻止SIGUSR1信号中断代码临界区。相当于是下面３步的原子操作
	 * 1、阻塞SIGUSR1信号
	 * 2、pause挂起当前进程
	 * 3、当捕捉一个信号并从信号处理函数返回时，sigsuspend解除对SIGUSR1的阻塞，并把进程的阻塞
	 * 信号集恢复为调用sigsuspend之前的值
	 */
	if (sigsuspend(&waitmask) != -1) {
		err_sys("sigsuspend error");
	}

	pr_mask("after return from sigsuspend：");

	/**
	 * 重置进程的阻塞信号集，解除对SIGINI的阻塞
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		err_sys("SIG_SETMASK error");
	}

	pr_mask("program exit：");

	exit(0);
}

static void			pr_mask(const char *str)
{
	int 		errno_save;
	sigset_t 	sigset;

	/**
	 * 信号操作函数会修改errno的值，这里先保存起来等调用完成后再恢复
	 */
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

static void			sig_int(int signo)
{
	pr_mask("\nin sig_int：");
}

/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./sigsuspend
program start：
in critical region： SIGINT
^C
in sig_int： SIGINT SIGUSR1
after return from sigsuspend： SIGINT
program exit：*/