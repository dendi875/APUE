/**
 * setsid函数实例，创建一个新的会话
 */

#include "apue.h"

int main(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {		/* 子进程中 */
		printf("Child process PID is %d\n", getpid());
		printf("Child process Group ID is %d\n", getpgid(0));
		printf("Child process SESSION ID is %d\n", getsid(0));
		sleep(10);	/* 父进程先退出，子进程成为孤儿进程，它不是组长进程 */

		setsid();

		printf("Changed：\n");
		printf("Child process PID is %d\n", getpid());
		printf("Child process Group ID is %d\n", getpgid(0));
		printf("Child process SESSION ID is %d\n", getsid(0));
		sleep(20);

		exit(0);
	}

	/* 父进程中 */
	exit(0);
}


/*调用了setsid后：
1、该进程的会话ID被设置为该进程的ID
2、该进程的进程组ID被设置为该进程的ID
3、该进程没有控制终端
4、如果该进程是组长进程（进程ID等于进程组ID），则出错；一般防止出错，通过先调用fork，然后使其父进程
终止，而子进程调用setsid，因为子进程的进程组ID是父进程的ID，子进程的ID是新分配的，所以子进程不可能
是组长进程。


setsid创建一个新的会话后有什么用？新的会话是脱离了控制终端，因为脱离了控制终端当控制终端关闭时（单击那个Ｘ时）进程还在运行

实验：
[dendi875@localhost Chapter-9-Process Relationships]$ ./my_setsid
[dendi875@localhost Chapter-9-Process Relationships]$ Child process PID is 2037
Child process Group ID is 2036
Child process SESSION ID is 1910
Changed：
Child process PID is 2037
Child process Group ID is 2037
Child process SESSION ID is 2037

ps ajx 查看
[dendi875@localhost ~]$ ps ajx | grep my_setsid
   1  2037  2037  2037 ?           -1 Ss     501   0:00 ./my_setsid
1929  2039  2038  1929 pts/1     2038 S+     501   0:00 grep my_setsid

？代表是无控制终端，PID为2037的进程是会话首进程（创建会话的进程），会话首进程总是一个进程组的组长进程
*/
