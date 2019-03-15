/**
 * clock_gettime函数实例
 */
#include "apue.h"
#include <time.h>

int main(void)
{
    struct timespec tsp;

    if (clock_gettime(CLOCK_REALTIME, &tsp) < 0) {
        err_sys("clock_gettime error");
    }
    printf("CLOCK_REALTIME：%ld %ld\n", tsp.tv_sec, tsp.tv_nsec);

    if (clock_gettime(CLOCK_MONOTONIC, &tsp) < 0) {
        err_sys("clock_gettime error");
    }
    printf("CLOCK_MONOTONIC：%ld %ld\n", tsp.tv_sec, tsp.tv_nsec);

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsp) < 0) {
        err_sys("clock_gettime error");
    }
    printf("CLOCK_PROCESS_CPUTIME_ID：%ld %ld\n", tsp.tv_sec, tsp.tv_nsec);

    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tsp) < 0) {
        err_sys("clock_gettime error");
    }
    printf("CLOCK_THREAD_CPUTIME_ID：%ld %ld\n", tsp.tv_sec, tsp.tv_nsec);

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-6-System Data Files and Information]$ ./my_clock_gettime
CLOCK_REALTIME：1552652243 126148690
CLOCK_MONOTONIC：211925 309339384
CLOCK_PROCESS_CPUTIME_ID：0 10235413
CLOCK_THREAD_CPUTIME_ID：0 10240003

注意：gcc编译时要加上-lrt
*/