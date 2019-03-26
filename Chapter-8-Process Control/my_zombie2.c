/**
 * 创建一个僵尸进程并用ps查看其状态
 */

#include "apue.h"

#ifdef SOLARIS
#define PSCMD "ps -a -o pid,ppid,s,tty,comm"
#else
#define PSCMD "ps -o pid,ppid,state,tty,command"
#endif

int main(void)
{
	pid_t	pid;

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {		/* 子进程中 */
		exit(0);
	}

	/* 父进程中 */
	sleep(4);
	system(PSCMD);

	exit(0);
}


/*实验：
[dendi875@localhost Chapter-8-Process Control]$ ./my_zombie2
PID  PPID S TT       COMMAND
1976  1975 S pts/0    -bash
2000  1976 S pts/0    ./my_zombie2
2001  2000 Z pts/0    [my_zombie2] <defunct>
2002  2000 R pts/0    ps -o pid,ppid,state,tty,command

注意：system执行了fork，exec，waitpid
*/