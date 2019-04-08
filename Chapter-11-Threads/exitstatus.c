/**
 *使用pthread_join来获取线程的退出状态
 */

#include "apue.h"
#include <pthread.h>

void *thr_fun1(void *arg)
{
    printf("thread 1 returning\n");
    return((void *)1);
}

void *thr_fun2(void *arg)
{
    printf("thread 2 exiting\n");
    pthread_exit((void *)2);
}

int main(void)
{
    pthread_t   tid1, tid2;
    int         err;
    void        *retval;

    err = pthread_create(&tid1, NULL, thr_fun1, NULL);   /* 创建一个新线程 */
    if (err != 0) {
        err_exit(err, "can't create thread 1");
    }
    err = pthread_create(&tid2, NULL, thr_fun2, NULL);   /* 再创建一个新线程 */
    if (err != 0) {
        err_exit(err, "can't create thread 2");
    }

    err = pthread_join(tid1, &retval);
    if (err != 0) {
        err_exit(err, "can't join with thread 1");
    }
    printf("thread 1 exit code %ld\n", (long)retval);

    err = pthread_join(tid2, &retval);
    if (err != 0) {
        err_exit(err, "can't join with thread 2");
    }
    printf("thread 2 exit code %ld\n", (long)retval);

    exit(0);
}

/*
如果进程中的任意线程调用了exit、_exit或_Exit，那么整个进程就会终止。
如果信号的默认处理动作是终止进程，那么，发送到线程的该信号就会终止整个进程。

单个线程可以通过３种方式退出
1、线程可以简单地从启动例程中返回，返回值是线程的退出码（在线程的运行函数中直接return）
2、线程调用pthread_exit退出
3、线程可以被同一进程中的其它线程取消。

int pthread_join(pthread_t thread, void **retval);
进程中的其它线程可以用pthread_join来访问pthread_exit函数的参数值。
调用线程将一直阻塞，直到指定的线程调用pthread_exit、或被取消、或从启动例程返回。
如果线程简单地从它的启动例程返回，retval就包含返回码。
如果线程被取消，由retval指向的内存单元就设置为PTHREAD_CANCELED

可以通过调用pthread_join自动把线程置于分离状态，这样资源就可以恢复。如果线程已经处于分离状态，
pthread_join会调用失败，返回EINVAL

如果对线程的返回值并不感兴趣，那么就可以把retval设置为NULL，在这种情况下，调用pthread_join函数
可以等待指定的线程终止，但并不获取线程的终止状态。

实验：
[dendi875@localhost Chapter-11-Threads]$ ./exitstatus
thread 2 exiting
thread 1 returning
thread 1 exit code 1
thread 2 exit code 2

从该程序可以看到，当一个线程通过调用pthread_exit退出时或从运行函数中返回时，进程中的其它线程
可以通过调用pthread_join函数来获得该线程的退出状态。
*/