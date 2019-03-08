/**
 *打印用户ID和组ID
 */
#include "apue.h"

int main(int argc, char **argv)
{
    printf("uid = %d, gid = %d\n", getuid(), getgid());

    exit(0);
}

/**
 * 实验：
 *[dendi875@localhost Chapter-1-System Overview]$ ./uidgid
 *uid = 504, gid = 505
 *
 * getpid() 返回调用进程的ID
 * getppid() 返回调用进程的父进程ID
 *
 * getuid() 返回调用进程的实际用户ID
 * getgid() 返回调用进程的实际组ID
 *
 * geteuid() 返回进程的有效用户ID
 * getegid() 返回进程的有效组ID
 */