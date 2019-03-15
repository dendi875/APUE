/**
 * clock_getres函数实例
 */

#include "apue.h"
#include <time.h>

int main(void)
{
    struct timespec res;

    if (clock_getres(1, &res) < 0) {
        err_sys("clock_getres error");
    }
    printf("seconds：%ld\n", res.tv_sec);
    printf("nanoseconds：%ld\n", res.tv_nsec);

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-6-System Data Files and Information]$ ./my_clock_getres
seconds：0
nanoseconds：1
*/