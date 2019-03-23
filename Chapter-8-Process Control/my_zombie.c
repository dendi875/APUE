/**
 * 僵尸进程实例
 */

#include "apue.h"

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 子进程中 */
        printf("I am child process pid：%d\tppid：%d\n", getpid(), getppid());
        printf("child process is exited.\n");
        exit(0);                /* 子进程正常退出 */
    }
    while (1) {                 /* 父进程中 */
        printf("I am parent process pid：%d\tppid：%d\n", getpid(), getppid());
        sleep(1);
    }
}


/*实验：
运行程序产生僵尸进程
[dendi875@localhost Chapter-8-Process Control]$ ./my_zombie
I am parent process pid：5141   ppid：3518
I am child process pid：5142    ppid：5141
child process is exited.

ps查看僵尸进程
[root@localhost ~]# ps -ef | grep zombie
dendi875  5141  3518  0 13:55 pts/0    00:00:00 ./my_zombie
dendi875  5142  5141  0 13:55 pts/0    00:00:00 [my_zombie] <defunct>

kill -9 僵尸进程ID，这样是销毁不了僵尸进程因为僵尸进程已经是死亡了，杀它的父进程就能销毁僵尸进程。

./my_zombie的父进程是shell进程
[dendi875@localhost Chapter-8-Process Control]$ echo $$
3518
*/
