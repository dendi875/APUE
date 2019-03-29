/**
 * 孤儿进程实例
 */

#include "apue.h"

static void pr_ids(char *);

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 子进程中 */
        pr_ids("child");
        sleep(3);   /* 睡眠3s，保证父进程先退出，子进程成为孤儿进程 */
        pr_ids("now child");
        exit(0);
    }

    /**
     * 父进程睡眠1s，保证子进程先运行
     * 在子进程还没有成为孤儿进程前打印父进程ID和子进程成为孤儿进程后打印的父进程ID做对比
     */
    sleep(1);
    pr_ids("parent");
    printf("parent process is exited.\n");
    exit(0);
}

static void pr_ids(char *name)
{
    printf("%s：pid = %ld, ppid = %ld, gpid = %ld, sid = %ld, tpgid = %ld\n",
            name, (long)getpid(), (long)getppid(), (long)getpgid(0), (long)getsid(0), (long)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}

/*实验：
[dendi875@localhost Chapter-8-Process Control]$ ./my_orphan
child：pid = 19597, ppid = 19596, gpid = 19596, sid = 17600, tpgid = 19596
parent：pid = 19596, ppid = 17600, gpid = 19596, sid = 17600, tpgid = 19596
parent process is exited.
[dendi875@localhost Chapter-8-Process Control]$ now child：pid = 19597, ppid = 1, gpid = 19596, sid = 17600, tpgid = 17600

命令：
$ ps -o pid,ppid,pgid,sid,tpgid,state,tty,comm
*/