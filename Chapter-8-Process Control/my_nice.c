/**
 * nice函数实例，获取当前进程的nice值
 */

#include "apue.h"

int main(void)
{
    int nzero;

#if defined(NZERO)
    nzero = NZERO;
#elif defined(_SC_NZERO)
    nzero = sysconf(_SC_NZERO);
#else
    #error NZERO undefined
#endif

    printf("NZERO = %d, nice(0) = %d\n", nzero, nice(0));

    exit(0);
}


/*实验：
[dendi875@localhost Chapter-8-Process Control]$ ./my_nice
NZERO = 20, nice(0) = 0

获取当前shell进程的ID
[dendi875@localhost Chapter-8-Process Control]$ echo $$
12081

查看指定进程ID的nice值和priority值
PID     USER    PR   NI VIRT  RES  SHR S  %CPU  %MEM TIME+   COMMAND
12081 dendi875  20   0  6872 1724 1408 S  0.0  0.2   0:00.68 bash*/