/**
 * 此程序说明如果使用不带信号处理功能的system实现版本（my_system.c），会带来哪些问题
 */

#include "apue.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

static void sig_int(int);
static void sig_chld(int);
int my_system(const char *);

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

/**
 * 不对信号处理的system实现版本
 */
int my_system(const char *cmdstring)
{
	pid_t	pid;
	int 	status;

	if (cmdstring == NULL) {
		return(1);
	}

	if ((pid = fork()) < 0) {
		status = -1;
	} else if (pid == 0) {	/* 子进程中 */
		execl("/bin/bash", "bash", "-c", cmdstring, (char *)0);
		_exit(127);			/* execl失败 */
	} else {	/* 父进程中 */
		while (waitpid(pid, &status, 0) < 0) {	/* 阻塞方式等待子进程终止 */
			if (errno != EINTR) {
				status = -1;
				break;
			}
		}
	}

	return(status);
}

/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_system2
a
Here is on line of text
.
1,%p
Here is on line of text
w temp.foo
24
q
caught SIGCHLD

再次运行程序
[dendi875@localhost Chapter-10-Signals]$ ./my_system2
a
hello, world
.
1,%p
hello, world
w temp.foo
13
^C
?
caught SIGINT
q
caught SIGCHLD


可以看到如果使用不带信号处理功能的system实现版本（my_system.c），那么父进程(my_system2的调用进程)
就接收到了SIGINT和SIGCHILD这两个信号。

SIGCHILD信号会使父进程对它真正想要等待的其它子进程造成干扰
SIGINT信号会使父进程中断，也会对父进程造成干扰

*/