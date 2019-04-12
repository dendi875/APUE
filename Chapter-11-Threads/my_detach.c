/**
 * pthread_detach函数实例
 *
 * pthread_detach函数作用是将线程标记为已分离。当处于分离状态的线程终止时，其资源将自动释放，
 * 而不需要另一个线程调用pthread_join来回收。
 *
 * 如果一个线程已经被标记为已分离，那么就不能再使用pthread_join来回收它
 */

#include "apue.h"
#include <pthread.h>

void *thr_fun1(void *arg)
{
    int n = 5;
    while (n--) {
        printf("thread count %d\n", n);
        sleep(1);
    }

    return((void *)1);
}

int main(void)
{
    pthread_t       tid1;
    int             err;
    void            *retval;

    /* 创建线程tid1 */
    err = pthread_create(&tid1, NULL, thr_fun1, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread 1");
    }

    /* 第一次运行注释掉下面这行，第二次再打开，分析两次结果 */
    pthread_detach(tid1);

    while (1) {
        err = pthread_join(tid1, &retval);  /* 阻塞等待线程终止，回收它的资源 */
        if (err != 0) {
            err_exit(err, "can't join width thread 1");
        }
        printf("thread 1 exit code %ld\n", (long)retval);

        sleep(1);
    }

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-11-Threads]$ ./my_detach
thread count 4
thread count 3
thread count 2
thread count 1
thread count 0
thread 1 exit code 1
can't join width thread 1: No such process

打开pthread_detach(tid1);的注释再编译运行
[dendi875@localhost Chapter-11-Threads]$ ./my_detach
can't join width thread 1: Invalid argument
*/