/**
 * 带信号处理功能的system版本的实现，与my_system2.c不带信号处理功能的实现差异为：
 * 1、system的调用者在等待完成命令时，会忽略SIGINT、SIGQUIT信号
 * 2、system的调用者如果正在waitpid捕捉其它子进程的SIGCHLD信号时，那么system执行命令所产生的
 * SIGCHLD信号对system的调用者来说是阻塞的。
 */

#include "apue.h"
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

int my_system(const char *);
static void sig_int(int);
static void sig_chld(int);

int main(void)
{
	struct sigaction int_act, chld_act;

	int_act.sa_handler = sig_int;
	sigemptyset(&int_act.sa_mask);
	int_act.sa_flags = 0;

	chld_act.sa_handler = sig_chld;
	sigemptyset(&chld_act.sa_mask);
	chld_act.sa_flags = 0;

	if (sigaction(SIGINT, &int_act, NULL) < 0) {
		err_sys("sigaction(SIGINT) error");
	}
	if (sigaction(SIGCHLD, &chld_act, NULL) < 0) {
		err_sys("sigaction(SIGCHLD) error");
	}
	if (my_system("/bin/ed") < 0) {
		err_sys("my_system() error");
	}

	exit(0);
}

static void sig_int(int signo)
{
	printf("caught SIGINT\n");
}

static void sig_chld(int signo)
{
	printf("caught SIGCHLD\n");
}

int my_system(const char *cmdstring)
{
	pid_t				pid;
	int 				status;
	struct sigaction 	ignore, saveintr, savequit;
	sigset_t			chldmask, savemask;

	if (cmdstring == NULL) {
		return(1);
	}

	/**
	 * fork后子进程继承了父进程的阻塞信号集和每个信号绑定的处理动作。
	 * execl后新的程序从调用进程那里继续了进程的阻塞信号集和每个信号绑定的处理动作。
	 *
	 * 父进程需要设置对SIGINT、SIGQUIT这两个信号的处理动作为忽略，并且保存当前这两个信号的处理动作
	 * 为在子进程中恢复成原来的动作及父进程调用waitpid返回后（execl执行完成）恢复成原来的动作做准备。
	 *
	 * 父进程还需将SIGCHLD信号加到当前进程的阻塞信号集中，并保存当前进程的阻塞信号集状态，目的也是
	 * 为在子进程中恢复成原来的阻塞信号集状态及父进程调用waitpid返回后（execl执行完成）恢复成原来的
	 * 阻塞信号集状态做准备。
	 */
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	if (sigaction(SIGINT, &ignore, &saveintr) < 0) {
	 	return(-1);
	}
	if (sigaction(SIGQUIT, &ignore, &savequit) < 0) {
	 	return(-1);
	}

	sigemptyset(&chldmask);
	sigaddset(&chldmask, SIGCHLD);
	if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0) {
	 	return(-1);
	}

	if ((pid = fork()) < 0) {
	 	status = -1;	/* fork失败 */
	} else if (pid == 0) {		/* 在子进程中 */
	 	/**
	 	 *子进程把对SIGINT和SIGQUIT信号的处理动作恢复成忽略之前的动作，
	 	 *阻塞信号号也恢复成对SIGCHLD阻塞之前的状态
	 	 */
	 	sigaction(SIGINT, &saveintr, NULL);
	 	sigaction(SIGQUIT, &savequit, NULL);
	 	sigprocmask(SIG_SETMASK, &savemask, NULL);

	 	execl("/bin/bash", "bash", "-c", cmdstring, (char *)0);
	 	_exit(127);	/* execl执行失败 */
	} else {	/* 父进程中 */
	 	while (waitpid(pid, &status, 0) < 0) {
	 		if (errno != EINTR) {
	 			status = -1;	/* 除了EINTR以外的错误返回-1 */
	 			break;
	 		}
	 	}
	}

	/**
	 *父进程把对SIGINT和SIGQUIT信号的处理动作恢复成忽略之前的动作，
	 *阻塞信号号也恢复成对SIGCHLD阻塞之前的状态
	 */
	if (sigaction(SIGINT, &saveintr, NULL) < 0) {
		return(-1);
	}
	if (sigaction(SIGQUIT, &savequit, NULL) < 0) {
		return(-1);
	}
	if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0) {
		return(-1);
	}

	return(status);
}

/*实验：
[dendi875@192 Chapter-10-Signals]$ ./system
a
Here is one line of text
.
1,$p
Here is one line of text
w temp.foo
25
q
caught SIGCHLD


[dendi875@192 Chapter-10-Signals]$ ./system
a
hello, world
.
1,$p
hello, world
w temp.foo
13
^C
?
q
caught SIGCHLD


[dendi875@192 Chapter-10-Signals]$ ./system
a
hello, world
.
1,$p
hello, world
w temp.foo
13
^C
?
^\^\^\
?
^\
?
q
caught SIGCHLD
*/