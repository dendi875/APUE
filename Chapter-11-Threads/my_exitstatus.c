/**
 *该程序主要是说明使用pthread_cancel来请求取消同一进程的其它线程
 */

#include "apue.h"
#include <pthread.h>

void *thr_fun1(void *arg)
{
    printf("thread 1 returning\n");
    return((void *)1);      /* 显示只想返回一个整数，但也要强转成void *类型 */
}

void *thr_fun2(void *arg)
{
    printf("thread 2 exiting\n");
    pthread_exit((void *)2);
}

void *thr_fun3(void *arg)
{
    while (1) {
        printf("thread 3 waiting\n");
        sleep(1);
    }
}

int main(void)
{
    pthread_t   tid1, tid2, tid3;
    int         err;
    void        *retval;

    err = pthread_create(&tid1, NULL, thr_fun1, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread 1");
    }
    err = pthread_join(tid1, &retval);   /* 阻塞等待指定线程退出 */
    if (err != 0) {
        err_exit(err, "can't join with thread 1");
    }
    printf("thread 1 exit code %ld\n", (long)retval);

    err = pthread_create(&tid2, NULL, thr_fun2, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread 2");
    }
    err = pthread_join(tid2, &retval);
    if (err != 0) {
        err_exit(err, "can't join with thread 2");
    }
    printf("thread 2 exit code %ld\n", (long)retval);

    err = pthread_create(&tid3, NULL, thr_fun3, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread 3");
    }
    sleep(3);
    pthread_cancel(tid3);    /* 请求取消thread 3 */
    err = pthread_join(tid3, &retval);
    if (err != 0) {
        err_exit(err, "can't join with thread 3");
    }
    printf("thread 3 exit code %ld\n", (long)retval);

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-11-Threads]$ ./my_exitstatus
thread 1 returning
thread 1 exit code 1
thread 2 exiting
thread 2 exit code 2
thread 3 waiting
thread 3 waiting
thread 3 waiting
thread 3 exit code -1

可以查到PTHREAD_CANCELED宏值为-1
[dendi875@localhost mysql]$ grep PTHREAD_CANCELED /usr/include/pthread.h
#define PTHREAD_CANCELED ((void *) -1)

注意：pthread_cancel并不等待线程终止，它只是提出请求。
*/