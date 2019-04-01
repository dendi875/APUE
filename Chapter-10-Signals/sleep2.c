/**
 * sleep的另一个不完整的实现，它避免了alarm和pause的竞争条件
 */
#include "apue.h"
#include <unistd.h>
#include <setjmp.h>

jmp_buf env_alrm;

static void sig_alrm(int);
unsigned int sleep2(unsigned int);

int main(void)
{
	sleep2(3);

	return 0;
}

static void sig_alrm(int signo)
{
	longjmp(env_alrm, 1);
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
