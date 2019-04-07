/**
 * sleep可靠的实现版本。它解决了sleep1.c版本的竞争条件的问题，也解决了tsleep2.c版本涉及与其它
 * 信号交互时会出现的问题
 */

#include "apue.h"
#include <unistd.h>
#include <signal.h>

unsigned int sleep3(unsigned int);
static void sig_alrm(int);
static void sig_int(int);

int main(void)
{
	unsigned int 		unslept;
	struct sigaction 	act;

	act.sa_handler = sig_int;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(SIGINT, &act, NULL) < 0) {
		err_sys("sigaction(SIGINT) error");
	}

	unslept = sleep3(5);
	printf("sleep2 returned：%u\n", unslept);

	exit(0);
}

unsigned int sleep3(unsigned int seconds)
{
	struct sigaction	newact, oldact;
	sigset_t			newmask, oldmask, suspmask;
	unsigned int 		unslept;

	/**
	 * 设置SIGALRM信号的处理动作，并保存上一个动作
	 */
	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldact);

	/**
	 * 将SIGALRM信号加入到当前进程的阻塞信号集中，并保存进程现有阻塞信号集的状态
	 */
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	/**
	 * 安装一个定时器，超时时产生SIGALRM信号
	 */
	alarm(seconds);
	suspmask = oldmask;

	/**
	 * 确保SIGALRM信号它一定不是阻塞的（有可能oldmask里原来SIGALRM就是阻塞的）
	 */
	sigdelset(&suspmask, SIGALRM);

	/**
	 * 使用suspmask信号集（SIGALRM不在这个信号集中）来阻塞当前进程，使进程挂起，
	 * 当捕捉到一个信号并从信号处理程序返回时sigsuspend返回，并且把当前进程的阻塞信号集恢复
	 * 到调用sigsuspend之前的状态（46代码那时的状态）
	 */
	sigsuspend(&suspmask);

	/**
	 * 当sigsuspend返回时，现在SIGALRM信号是阻塞的，
	 * 一个进程只能有一个定时器，如果在调用alarm时，之前为进程注册的闹钟时间还没有超时，则
	 * 本次alrm函数的返回值为上次闹钟的余留值。以前注册的闹钟时间则被新值代替。而且如果本次
	 * 调用的alarm函数的参数为0，则取消以前的闹钟时间。
	 */
	unslept = alarm(0);

	/**
	 * 把对SIGALRM信号处理动作恢复到绑定sig_alrm程序之前
	 */
	sigaction(SIGALRM, &oldact, NULL);

	/**
	 * 恢复进程的阻塞信号集为设置SIGALRM为阻塞信号集之前
	 */
	sigprocmask(SIG_SETMASK, &oldmask, NULL);


	return(unslept);
}

static void sig_alrm(int signo)
{
	/* 什么都不做仅仅是当返回时为了唤醒sigsuspend() */
}

static void sig_int(int signo)
{
	int 			i, j;
	volatile int 	k;

	/**
	 * 可以调整循环终止条件值，使for循环的时间超过5秒
	 */
	printf("\nsig_int starting\n");
	for (i = 0; i < 300000; i++) {
		for (j = 0; i < 4000; j++) {
			k += i * j;
		}
	}

	printf("sig_int finished\n");
}