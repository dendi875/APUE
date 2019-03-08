/**
 * 打印进程ID
 */
#include "apue.h"

int main(int argc, char **argv)
{
    printf("hello world from process ID %ld\n", (long int) getpid());

    exit(0);
}

/**
 * 实验：
 *
 * [dendi875@localhost Chapter-1-System Overview]$ ./getpid
 * hello world from process ID 16464
 */