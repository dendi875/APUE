/**
 * 线程栈属性设置示例
 */

#include "apue.h"
#include <pthread.h>

#define    NTHREADS    4
#define    N           1000
#define    MEGEXTRA    1000000  /* 额外的 */

pthread_attr_t         attr;

void *dowork(void *arg)
{
    double A[N][N];
    int i, j, tid;
    size_t  mystacksize;

    tid = (int)arg;
    pthread_attr_getstacksize(&attr, &mystacksize);
    printf("Thread %d：stack size = %lu bytes\n", tid, mystacksize);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = ((i * j) / 3.452) + (N - i);
        }
    }

    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t       threads[NTHREADS];
    size_t          stacksize;
    int err, t;

    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &stacksize);
    printf("Default stack size = %lu bytes\n", stacksize);
    stacksize = sizeof(double) * N * N + MEGEXTRA;
    printf("Amount of stack needed per thread = %lu bytes\n", stacksize);
    pthread_attr_setstacksize(&attr, stacksize);
    printf("Creating threads with stack size = %lu bytes\n", stacksize);

    for (t = 0; t < NTHREADS; t++) {
        err = pthread_create(&threads[t], &attr, dowork, (void *)t);
        if (err != 0) {
            err_exit(err, "can't create thread %d", t);
        }
    }

    printf("Created %d thread.\n", t);

    pthread_exit(NULL);
}

/*
实验：
[dendi875@localhost Chapter-12-Thread Control]$ ./my_stack_attr
Default stack size = 10485760 bytes
Amount of stack needed per thread = 9000000 bytes
Creating threads with stack size = 9000000 bytes
Created 4 thread.
Thread 3：stack size = 9000000 bytes
Thread 2：stack size = 9000000 bytes
Thread 1：stack size = 9000000 bytes
Thread 0：stack size = 9000000 bytes

[dendi875@localhost Chapter-12-Thread Control]$ ulimit -s
10240（kbytes）

10485760 bytes == 10M
*/