/**
 *取消点函数实例；查看man 7 pthreads，man 7 pthread_testcancel
 */

#include "apue.h"
#include <pthread.h>

void *thr_fun1(void *arg)
{
    int i = 0;
    printf("thread 1 tid = %lu\n", (unsigned long)pthread_self());
    while (1) {
        i++;
        // 第二次运行时打开下面的注释
        // sleep(1);   /* 创建取消点 */
    }

    return((void *)0);
}

int main(void)
{
    pthread_t       tid1;
    int             err;
    void            *retval;

    /* 打印主控线程信息 */
    printf("main thread tid = %lu\n", (unsigned long)pthread_self());

    /* 创建一个线程 */
    err = pthread_create(&tid1, NULL, thr_fun1, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread 1");
    }

    /* 给tid1线程一个开始的机会 */
    sleep(2);

    /* 向tid1线程发送一个取消请求 */
    err = pthread_cancel(tid1);
    if (err != 0) {
        err_exit(err, "pthread_cancel thread 1 error");
    }

    /* 使主控线程死循环不退出终止，方便ps -eLf观察两个线程是否终止 */
    while (1) {
        sleep(1);
    }

    exit(0);
}

/*
运行程序
[dendi875@localhost Chapter-11-Threads]$ ./my_cancellation_point
main thread tid = 3078067904
thread 1 tid = 3078065008

另开一个终端查看线程是否终止
[dendi875@localhost ~]$ ps -eLf | grep my_cancellation_point
dendi875 10028  2778 10028  0    2 10:52 pts/0    00:00:00 ./my_cancellation_point
dendi875 10028  2778 10029 99    2 10:52 pts/0    00:00:43 ./my_cancellation_point

可以看到虽然一个线程给另一个线程发送了取消请求但是另一个线程并没有终止掉。是因为请求并不是马上处理的
它需要有一个取消点。可以看man 2 pthread_cancel，man 7 pthreads可以看到可以创建取消点的函数有哪些，
常见的pthread_testcancel和sleep能创建取消点。

修改程序调用一个创建取消点函数在线程函数中加入sleep(1);
重新编译运行程序
[dendi875@localhost Chapter-11-Threads]$ ./my_cancellation_point
main thread tid = 3078833856
thread 1 tid = 3078830960

开另一个终端查看线程是否存在
[dendi875@localhost ~]$ ps -eLf | grep my_cancellation_point
dendi875 10105  2778 10105  0    1 11:02 pts/0    00:00:00 ./my_cancellation_point
dendi875 10110  3887 10110  0    1 11:02 pts/1    00:00:00 grep --color=auto my_cancellation_point

现在可以看到tid1线程已经被终止了

*/
