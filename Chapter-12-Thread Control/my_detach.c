/**
 * 创建一个分离态的线程
 */

#include "apue.h"
#include <pthread.h>

void *thr_fun(void *arg)
{
    int n = 5;
    while (n--) {
        printf("thread count n = %d\n", n);
        sleep(1);
    }
    pthread_exit((void *)0);
}

int main(void)
{
    int                 err;
    pthread_t           tid;
    pthread_attr_t      attr;   /* 如果不init初始则结构体是个垃圾值 */
    int                 detachstate;

    pthread_attr_init(&attr);

    /* 打印分离状态属性 */
    err = pthread_attr_getdetachstate(&attr, &detachstate);
    if (err != 0) {
        err_exit(err, "pthread_attr_getdetachstate");
    }
    printf("Detach state = %s\n",
        (detachstate == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
        (detachstate == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
        "???");

    /* 设置属性为分离状态 */
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err != 0) {
        err_exit(err, "pthread_attr_setdetachstate");
    }
    err = pthread_create(&tid, &attr, thr_fun, NULL);
    if (err != 0) {
        err_exit(err, "pthread_create");
    }
    /* 再次打印分离状态属性 */
    err = pthread_attr_getdetachstate(&attr, &detachstate);
    if (err != 0) {
        err_exit(err, "pthread_attr_getdetachstate");
    }
    printf("Detach state = %s\n",
        (detachstate == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
        (detachstate == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
        "???");

    pthread_attr_destroy(&attr);    /* 释放线程属性资源 */

    sleep(10);

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-12-Thread Control]$ ./my_detach
Detach state = PTHREAD_CREATE_JOINABLE
Detach state = PTHREAD_CREATE_DETACHED
thread count n = 4
thread count n = 3
thread count n = 2
thread count n = 1
thread count n = 0
*/
