/**
 * vfork函数实例
 */

#include "apue.h"

int globvar = 6;    /* 全局变量存储在虚拟地址空间中的初始化数据段中 */

int main(void)
{
    int var;    /* 自动变量存储在虚拟地址空间中的栈中 */
    pid_t pid;

    var = 88;
    printf("before vfork\n");
    if ((pid = vfork()) < 0) {
        err_sys("vfork error");
    } else if (pid == 0) {  /* 子进程的处理逻辑 */
        globvar++;
        var++;
        _exit(0);
    }

    /* 父进程从这继续运行 */
    printf("pid = %ld, globvar = %d, var = %d\n", (long)getpid(), globvar, var);
    exit(0);
}


/*vfork并不将父进程地址空间完全复制到子进程中，因为子进程会立即调用exec或exit，于是也就不会引用该地址空间。不过在子进程调用exec或exit之前，它在父进程中运行。

vfork与fork之间另一个区别是：vfork保证子进程先运行，在它调用exec或exit之后父进程才可能被调度运行，
当子进程调用exec或exit任意一个函数时，父进程会恢复运行。

实验：
[dendi875@localhost Chapter-8-Process Control]$ ./vfork1
before vfork
pid = 15392, globvar = 7, var = 89

从结果是可以看到因为在子进程调用exec或exit之前子进程是在父进程虚拟内存地址空间中运行的，所以导致
父进程虚拟地址空间初始化数据段中的globvar变量被修改，父进程虚拟地址空间中的栈段中的var自动变量
也被修改*/