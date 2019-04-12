/**
 *使用互斥量来进行多线程的同步，来保护共享资源
 */

#include "apue.h"
#include <pthread.h>

int globalvar;

void *thr_fun1(void *arg)
{
    int i, j;

    for (i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);
        j = globalvar;
        j = j + 1;
        printf(".");
        fflush(stdout);
        sleep(1);
        globalvar = j;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(void)
{
    pthread_t   tid1;
    int         err;
    int         i;

    /* 保护的资料是静态的变量也可以用 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; */
    pthread_mutex_t mutex;

    /* 动态的初始化互斥量 */
    pthread_mutex_init(&mutex, NULL);

    err = pthread_create(&tid1, NULL, thr_fun1, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread 1");
    }
    for (i = 0; i < 20; i++) {
        /**
         * 锁定指定的互斥量，如果该互斥量已经被其它线程锁定，则该调用将会被阻塞到线程解锁mutex，
         * 然后再去抢占锁，如果抢占失败则继续阻塞等待
         */
        pthread_mutex_lock(&mutex);

        globalvar =  globalvar + 1;

        /**
         *线程解锁自己占用的互斥量。一个线程完成了对共享数据的使用后，需要解锁自己占用的互斥量让
         *其它线程获得互斥量后对数据进行操作，如果互斥量已经被解锁，或互斥量被另一个线程占用则
         *pthread_mutex_unlock出错
         */
        pthread_mutex_unlock(&mutex);

        printf("o");
        fflush(stdout);
        sleep(1);
    }
    err = pthread_join(tid1, NULL);
    if (err != 0) {
        err_exit(err, "can't join with thread 1");
    }

    /**
     * 释放不需要再使用的互斥量
     * 一旦使用 pthread_mutex_init() 初始化了互斥对象，就应使用 pthread_mutex_destroy() 消除它
     */
    pthread_mutex_destroy(&mutex);

    printf("\nglobalvar = %d\n", globalvar);

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-11-Threads]$ time ./thread3
o....................ooooooooooooooooooo
globalvar = 40

real    0m39.022s
user    0m0.001s
sys     0m0.004s

代码可能的执行顺序

thr_fun1() 线程               主线程
j=globalvar;
j=j+1;
printf(".");
fflush(stdout);
sleep(1);                     globalvar=globalvar+1;
globalvar=j;
*/