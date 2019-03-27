/**
 * getpgid函数实例；获取一个进程的进程组ID
 */

#include "apue.h"

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        printf("Child process PID is %d\n", getpid());
        printf("Child process Group ID is %d\n", getpgrp());
        printf("Child process Group ID is %d\n", getpgid(getpid()));
        printf("Child process Group ID is %d\n", getpgid(0));
        exit(0);
    }

    sleep(3);
    printf("Parent process PID is %d\n", getpid());
    printf("Parent process Group ID is %d\n", getpgid(0));

    return 0;
}

/*实验：
[dendi875@localhost Chapter-9-Process Relationships]$ ./my_getgpid
Child process PID is 27784
Child process Group ID is 27783
Child process Group ID is 27783
Child process Group ID is 27783
Parent process PID is 27783
Parent process Group ID is 27783*/