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