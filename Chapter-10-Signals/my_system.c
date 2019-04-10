/**
 * 不带信号处理功能的system实现版本
 */

#include "apue.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int my_system(const char *);
void pr_exit(int);

int main(void)
{
	int status;

	if ((status = my_system("date")) < 0) {
		err_sys("my_system error");
	}
	pr_exit(status);

	if ((status = my_system("nosuchcommand")) < 0) {
		err_sys("my_system error");
	}
	pr_exit(status);

	if ((status = my_system("who; exit 44")) < 0) {
		err_sys("my_system error");
	}

	pr_exit(status);

	exit(0);
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

void pr_exit(int status)
{
	if (WIFEXITED(status)) {
		printf("normal termination, exit status = %d\n",
				WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("abnormal termination, signal number = %d%s\n",
				WTERMSIG(status),
#ifdef	WCOREDUMP
				WCOREDUMP(status) ? " (core file generated) " : "");
#else
				"");
#endif
	} else if (WIFSTOPPED(status)) {
		printf("child stopped, signal number = %d\n",
				WSTOPSIG(status));
	}
}

/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_system
2019年 04月 06日 星期六 23:24:39 CST
normal termination, exit status = 0
bash: nosuchcommand: command not found
normal termination, exit status = 127
dendi875 pts/0        2019-04-06 21:37 (192.168.208.1)
dendi875 pts/1        2019-04-06 21:38 (192.168.208.1)
normal termination, exit status = 44
*/