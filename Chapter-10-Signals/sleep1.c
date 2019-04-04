/**
 * sleep简化版本不完整的实现
 */

#include "apue.h"
#include <unistd.h>

static void sig_alrm(int);
unsigned int sleep1(unsigned int);

int main(void)
{
	sleep1(3);

	return 0;
}

static void sig_alrm(int signo)
{
	/* nothing to do, just return to wake up the pause */
}

unsigned int sleep1(unsigned int seconds)
{
	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		return(seconds);
	}
	alarm(seconds);	 	/* 设置一个定时器，当定时器超时时会发送SIGALRM信号 */
	pause();			/* 使调用进程挂起直到捕捉到一个信号 */
	return(alarm(0));	/* 关闭定时器 */
}

/**
 *这种实现sleep的方式是有问题的，alarm和pause之间存在一个竞争条件，在一个繁忙的系统中，可能
 *在调用pause之前alarm已经超时并且调用了sig_alrm处理程序，这种情况下在调用pause后，如果没有
 *捕捉到其它信号，则进程将被pause永远挂起。
 */