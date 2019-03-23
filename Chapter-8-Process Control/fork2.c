/**
 * 使用fork两次来实现非阻塞但不会产生僵尸进程的实例
 */

#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {                 /* 第一次fork的子进程 */
        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid > 0) {              /* 第二次fork的父进程，也是第一次fork的子进程*/
            exit(0);
        }

        /**
         * 因为fork不能确定父子进程谁先运行，所以在fork之后睡眠2秒，使父进程比子进程先终止，
         * 从而到达第二次的子进程的父进程为init进程。
         */
        sleep(2);
        printf("second child, parent pid = %ld\n", (long)getppid());
        exit(0);
    }

    if (waitpid(pid, NULL, 0) != pid) {             /* 这里是等待回收第一次fork出来的子进程 */
        err_sys("waitpid error");
    }

    /**
     * 这里是原始进程（也就是第一次fork后的父进程），这里继续向下执行。
     * 注意：这里的输出在printf("second child, parent pid = %ld\n", (long)getppid());这个输出前面
     */
    printf("first parent pid = %ld\n", (long)getpid());

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-8-Process Control]$ ./fork2
first parent pid = 6627
[dendi875@localhost Chapter-8-Process Control]$ second child, parent pid = 1*/