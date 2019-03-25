/**
 * system函数实例
 */

#include "apue.h"

void pr_exit(int);

int main(void)
{
	int status;

	if ((status = system("date")) < 0) {
		err_sys("system() error");
	}
	pr_exit(status);

	if ((status = system("nosuchcommand")) < 0) {
		err_sys("system() error");
	}
	pr_exit(status);

	if ((status = system("who; exit 44")) < 0) {
		err_sys("system() error");
	}
	pr_exit(status);

	exit(0);
}

void pr_exit(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termination, signal number = %d%s\n",
            WTERMSIG(status),
#ifdef WCOREDUMP
            WCOREDUMP(status) ? "（core file generated）" : "");
#else
            "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stoped, signal number = %dn", WSTOPSIG(status));
    }
}


/*实验：
[dendi875@localhost Chapter-8-Process Control]$ ./systest1
2019年 03月 25日 星期一 21:20:51 CST
normal termination, exit status = 0
sh: nosuchcommand: command not found
normal termination, exit status = 127
root     pts/0        2019-03-25 20:25 (192.168.208.1)
normal termination, exit status = 44*/