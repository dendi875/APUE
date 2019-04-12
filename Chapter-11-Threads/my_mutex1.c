/**
 *该例子演示了使用一个线程处理一个点积（dot product）计算。
 */

#include "apue.h"
#include <pthread.h>

/**
 * 该结构体允许函数dotproduct访问其中的输入数据，以及将其输出放入结构体中
 */
struct dotdata {
    double  *a;
    double  *b;
    double  sum;
    int     vectorlen; /* 向量长度 */
};

/* 定义全局变量和互斥量 */
#define NUMTHREADS      4
#define VECTORLEN       10000000

struct dotdata dostr;

void *dotproduct(void *arg)
{
    int i;
    double mysum;

    for (i = 0; i < NUMTHREADS * VECTORLEN; i++) {
        mysum += ((dostr.a)[i] * (dostr.b)[i]);
    }

    dostr.sum = mysum;

    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t   tid;
    int         err;
    int         *retval;
    double      *a, *b;
    int         i;

    /* 对dostr进行初始化 */
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

    /* 创建线程 */
    err = pthread_create(&tid, NULL, dotproduct, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread");
    }

    /* 阻塞等待线程终止 */
    err = pthread_join(tid, NULL);
    if (err != 0) {
        err_exit(err, "can't join whith thread");
    }

    printf("sum = %f\n", dostr.sum);
    free(a);
    free(b);

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-11-Threads]$ time ./my_mutex1
sum = 40000000.000000

real    0m15.499s
user    0m0.271s
sys     0m2.042s
*/