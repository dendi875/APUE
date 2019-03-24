/**
 * 解释器与解释器文件实例
 */

#include "apue.h"
#include <sys/wait.h>

int main(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {			/* 子进程执行逻辑 */
		if (execl("/home/dendi875/bin/testinterp",
			"testinterp", "myarg1", "MY ARG2", (char *)0) < 0) {
			err_sys("execl error");
		}
	}

	if (waitpid(pid, NULL, 0) < 0) {			/* 父进程阻塞等待子进程终止 */
		err_sys("waitpid error");
	}

	exit(0);
}


/*实验：

[dendi875@localhost Chapter-8-Process Control]$ cat /home/dendi875/bin/testinterp
#! /home/dendi875/bin/echoarg foo
[dendi875@localhost Chapter-8-Process Control]$ ll !$
ll /home/dendi875/bin/testinterp
-rwxrw-r--. 1 dendi875 dendi875 34 3月  24 19:41 /home/dendi875/bin/testinterp
[dendi875@localhost Chapter-8-Process Control]$ ./exec2
argv[0]：/home/dendi875/bin/echoarg
argv[1]：foo
argv[2]：/home/dendi875/bin/testinterp
argv[3]：myarg1
argv[4]：MY ARG2*/