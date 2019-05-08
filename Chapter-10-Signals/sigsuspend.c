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

/*
实验：
[dendi875@localhost Chapter-10-Signals]$ ./sigsuspend
program start：
in critical region： SIGINT
^C
in sig_int： SIGINT SIGUSR1
after return from sigsuspend： SIGINT
program exit：

注意：sigsuspend挂起进程期间会用waitmask信号集暂时替代进程的阻塞信号集，所以发送 SIGINT 信号才能被递送
到进程中。
以本程序为例：该进程的阻塞信号集状态经过了6次变动
1、调用 sigprocmask(SIG_BLOCK, &newmask, &oldmask) 之前进程阻塞信号集是空的（64位全为0）
2、调用 sigprocmask(SIG_BLOCK, &newmask, &oldmask) 之后进程阻塞信号集中只有SIGINT是阻塞的
（SIGINT对应位为1）
3、调用 sigsuspend(&waitmask) 挂起进程期间阻塞信号集中只有SIGUSR1是阻塞的（SIGUSR1对应位为1），
为什么SIGINT不是阻塞的因为进程阻塞信号集状态被sigsuspend调用暂时替换了，所以我们发送了 SIGINT 信号才能递送到进程
4、当捕捉到一个信号在执行信号处理函数期间被捕捉的信号也自动加到入进程的阻塞信号集中，这时当前进程的阻塞信号集中SIGINT和SIGUSR1信号是阻塞的。
5、当捕捉到一个信号从该信号处理函数返回后sigsuspend会恢复之前的阻塞信号集，这时进程的阻塞信号集
中只有SIGINT是阻塞的（SIGINT对应位为1）
6、当调用 sigprocmask(SIG_SETMASK, &oldmask, NULL) 恢复之前的阻塞信号集后，这时进程的阻塞信号集
是空的（64位都是0）
*/