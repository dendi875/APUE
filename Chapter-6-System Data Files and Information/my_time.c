/**
 * time函数使用实例
 */

#include "apue.h"
#include <time.h>

int main(void)
{
    time_t cur_time, *ptr;

    ptr = &cur_time;
    time(ptr);
    printf("cur_time = %ld\n", time(NULL));
    printf("cur_time = %ld\n", *ptr);

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-6-System Data Files and Information]$ ./my_time
cur_time = 1552650113
cur_time = 1552650113*/