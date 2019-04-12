/**
 *该例子演示了多线程使用互斥量处理一个点积（dot product）计算。共享资源数据通过一个可全局访问的
 *数据结构被所有线程使用，每个线程处理数据的不同部分，主控线程等待其它线程计算完成后再输出结果。
 *
 *与my_mutex1.c使用单一线程所需时间对比
 */

#include "apue.h"
#include <pthread.h>

/**
 * 该结构体允许函数dotproduct访问其中的输入数据，以及将其输出放入结构体中
 */
struct dotdata {
    double      *a;
    double      *b;
    double      sum;
    int         vectorlen;  /* 每个线程处理的向量长度 */
};

/* 定义全局变量和互斥量 */
#define  NUMTHREADS     4
#define  VECTORLEN      10000000

struct dotdata          dostr;
pthread_t               callThr[NUMTHREADS];    /* 保存线程ID的数组 */
pthread_mutex_t        mutexsum;

void *dotproduct(void *arg)
{
    int i, offset, len, start, end;
    double mysum, *x, *y;

    x = dostr.a;
    y = dostr.b;

    offset = (int)arg;
    len = dostr.vectorlen;
    start = offset * len;
    end = start + len;

    mysum = 0;
    for (i = start; i < end; i++) {
        mysum += (x[i] * y[i]);
    }

    pthread_mutex_lock(&mutexsum);      /* 锁住互斥量 */
    dostr.sum += mysum;                 /* 操作共享数据 */
    pthread_mutex_unlock(&mutexsum);    /* 解锁互斥量 */

    pthread_exit((void *)0);            /* 线程终止 */
}

/**
 * 主线程创建所有工作线程，然后打印输出结果。在创建线程之前需要对结构体中的输出数据初始化。
 * 主线程等待所有线程都完成工作后，打印输出值。
 * 还需注意不再需要做free处理
 */
int main(void)
{
    double              *a;
    double              *b;
    int                 err;
    void                *retval;
    int                 i;
    pthread_attr_t      attr;


    /* 初始化结构体 */
    a = (double *)malloc(NUMTHREADS * VECTORLEN * sizeof(double));
    b = (double *)malloc(NUMTHREADS * VECTORLEN * sizeof(double));

    for (i = 0; i < NUMTHREADS * VECTORLEN; i++) {
        a[i] = 1.0;
        b[i] = a[i];
    }

    dostr.a = a;
    dostr.b = b;
    dostr.sum = 0;
    dostr.vectorlen = VECTORLEN;

    /* 初始化互斥量 */
    pthread_mutex_init(&mutexsum, NULL);

    /* 设置将要创建的线程属性为非分离态 */
    err = pthread_attr_init(&attr);
    if (err != 0) {
        err_exit(err, "pthread_attr_init error");
    }
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    if (err != 0) {
        err_exit(err, "pthread_attr_setdetachstate error");
    }

    /* 创建线程 */
    for (i = 0; i < NUMTHREADS; i++) {
        err = pthread_create(&callThr[i], &attr, dotproduct, (void *)i);
        if (err != 0) {
            err_exit(err, "can't create thread %d", i);
        }
    }

    /* 阻塞等待所有线程都完成了工作，然后回收所有线程资源 */
    for (i = 0; i < NUMTHREADS; i++) {
        err = pthread_join(callThr[i], &retval);
        if (err != 0) {
            err_exit(err, "can't join with thread %d", i);
        }
    }

    /* 打印结果 */
    printf("sum = %f\n", dostr.sum);

    /* 释放不需要的资源 */
    free(a);
    free(b);
    pthread_mutex_destroy(&mutexsum);
    pthread_attr_destroy(&attr);

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-11-Threads]$ time ./my_mutex2
sum = 40000000.000000

real    0m2.698s
user    0m0.186s
sys     0m2.488s


dotproduct函数中各变量的变化情况
offset 0    1   2   3
len    100  100 100 100
start  0    100 200 300
end    100  200 300 400
*/