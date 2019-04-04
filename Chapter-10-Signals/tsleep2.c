/**
 * 在一个捕捉其它信号的程序中调用sleep，
 * 这个例子说明了当涉及与其他信号的交互时，sleep2.c实现sleep函数的方式还是有问题的
 */
#include "apue.h"
#include <setjmp.h>
#include <unistd.h>
#include <signal.h>

jmp_buf env_alrm;

static void sig_alrm(int);
static void sig_int(int);
unsigned int sleep2(unsigned int);

int main(void)
{
	unsigned int unslept;

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		err_sys("signal(SIGINT) error");
	}
	unslept = sleep2(5);
	printf("sleep2 returned：%u\n", unslept);

	exit(0);
}

static void sig_alrm(int signo)
{
	longjmp(env_alrm, 1);
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

unsigned int sleep2(unsigned int seconds)
{
	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		return(seconds);
	}
	if (setjmp(env_alrm) == 0) {
		alarm(seconds);
		pause();
	}
	return(alarm(0));
}

/*实验：
比如在运行程序后的2秒钟时按ctrl+c键，捕捉到SIGINT信号开始执行sig_int，但sig_int这个函数里for循环
需要5秒但闹钟超时还剩3秒，这时3秒过后闹钟超时已到就执行sig_alrm函数，然后sleep2返回到调用它的地方
unslept = sleep2(5)。这时sig_int这个信号处理程序就被提早终止了。

[dendi875@localhost Chapter-10-Signals]$ ./tsleep2
^C
sig_int starting
sleep2 returned：0*/