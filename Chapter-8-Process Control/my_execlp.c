/**
 * execlp函数实例
 */

#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 子进程中执行的代码 */
        if (execlp("ps", "ps", "-o", "pid,ppid,pgrp,session,tpgid,comm", (char *)0) < 0) {
            err_sys("execlp error");
        }
    }
    if (waitpid(pid, NULL, 0) < 0) {    /* 父进程阻塞等待子进程结束 */
        err_sys("wait error");
    }

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-8-Process Control]$ ./my_execlp
PID  PPID  PGRP  SESS TPGID COMMAND
3518  3517  3518  2768  8304 bash
8304  3518  8304  2768  8304 my_execlp
8305  8304  8304  2768  8304 ps*/