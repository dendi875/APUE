/**
 * 在一个捕捉其它信号的程序中调用sleep2
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
比如在运行程序后的2秒钟时按ctrl+c键
[dendi875@localhost Chapter-10-Signals]$ ./tsleep2
^C
sig_int starting
sleep2 returned：0*/