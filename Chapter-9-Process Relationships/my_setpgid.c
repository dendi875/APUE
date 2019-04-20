/**
 * setpgid函数实例；改变一个进程的进程组ID
 */

#include "apue.h"

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 子进程中 */
        printf("Child process PID is %d\n", getpid());
        printf("Child process Group ID is %d\n", getpgid(0));
        sleep(5);
        printf("Child process Group ID changed is %d\n", getpgid(0));
        exit(0);
    }

    sleep(1);    /* 睡眠1秒，先让子进程运行 */
    /**
     * 父进程修改子进程的组ID为子进程ID本身，子进程自立门户，成为进程组组长
     */
    setpgid(pid, pid);

    sleep(5);   /* 睡眠5秒，让子进程运行，子进程打印进程组ID修改后的值 */

    printf("Parent process PID is %d\n", getpid());         /* 父进程的PID */
    printf("Parent process PPID is %d\n", getppid());       /* 父进程的PPID */
    printf("Parent process Group ID is %d\n", getpgid(0));  /* 父进程的进程组ID */

    setpgid(getpid(), getppid());   /* 修改父进程的组ID为父进程的父进程的PID */
    printf("Parent process Group ID changed is %d\n", getpgid(0)); /* 查看父进程组ID修改后的值 */

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-9-Process Relationships]$ ./my_setpgid
Child process PID is 27966
Child process Group ID is 27965
Child process Group ID changed is 27966
Parent process PID is 27965
Parent process PPID is 24778
Parent process Group ID is 27965
Parent process Group ID changed is 24778*/