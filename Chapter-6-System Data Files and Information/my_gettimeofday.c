/**
 * gettimeofday函数实例
 */

#include "apue.h"
#include <sys/time.h>

int main(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    printf("seconds = %lld, microseconds = %lld\n", (long long)tv.tv_sec, (long long)tv.tv_usec);

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-6-System Data Files and Information]$ ./my_gettimeofday
seconds = 1553594122, microseconds = 465399

gettimeofday和time一样获取当前时间，但比time精度要高（到微秒）*/