/**
 * 读写锁实例程序
 *
 * 有3个线程不定时“写”全局资源，5个线程不定时“读”全局资源
 */

#include "apue.h"
#include <pthread.h>

#define NUMTHREADS  8   /* 共线程数量 */
#define WRTHREADS   3   /* 写线程数量 */
#define RDTHREADS   5   /* 读线程数量 */

/* 定义全局资源和读写锁变量 */
int counter;
pthread_rwlock_t    rwlock;


/**
 *写操作线程函数
 */
void *thr_write(void *arg)
{
    int t;
    int i = (int)arg;

    while (1) {
        t = counter;

        pthread_rwlock_wrlock(&rwlock);     /* 试图于写模式对读写锁进程加锁 */
        printf("wirte thread i = %d, tid = %lu, counter = %d, ++counter = %d\n",
                i, (unsigned long)pthread_self(), t, ++counter);   /* 操作全局资源 */
        pthread_rwlock_unlock(&rwlock);     /* 解锁读写锁 */

        usleep(6000);   /* 睡眠6000微秒 */
    }

    pthread_exit((void *)0);
}

/**
 * 读操作线程函数
 */
void *thr_read(void *arg)
{
    int i = (int)arg;

    while (1) {
        pthread_rwlock_rdlock(&rwlock);    /* 试图于读模式对读写锁进程加锁 */
        printf("read thread i = %d, tid = %lu, counter = %d\n",
                i, (unsigned long)pthread_self(), counter);     /* 读取全局资源 */
        pthread_rwlock_unlock(&rwlock);    /* 解锁读写锁 */

        /**
         * 让当前线程暂时挂起，暂时放弃CPU，让出CPU时间片给其它线程使用。从而到达多个线程交替
         * 执行，不是让某个线程一直占用CPU
         */
        usleep(3000);
    }

    return((void *)0);
}

/**
 * 主控线程它负责初始化读写锁，创建线程，销毁线程
 */
int main(void)
{
    pthread_t   tid[NUMTHREADS];
    int         err;
    int         i;

    pthread_rwlock_init(&rwlock, NULL); /* 初始化读写锁（分配读写锁资源），使用读写锁的默认属性 */

    /* 创建3个写线程 */
    for (i = 0; i < WRTHREADS; i++) {
        err = pthread_create(&tid[i], NULL, thr_write, (void *)i);
        if (err != 0) {
            err_exit(err, "can't create thread %d", i);
        }
    }

    /* 创建5个读线程 */
    for (i = WRTHREADS; i < NUMTHREADS; i++) {
        err = pthread_create(&tid[i], NULL, thr_read, (void *)i);
        if (err != 0) {
            err_exit(err, "can't create thread %d", i);
        }
    }

    /* 阻塞等待所有线程终止，回收资源 */
    for (i = 0; i < NUMTHREADS; i++) {
        err = pthread_join(tid[i], NULL);
        if (err != 0) {
            err_exit(err, "can't join with thread %d", i);
        }
    }

    pthread_rwlock_destroy(&rwlock);    /* 销毁读写锁资源 */

    exit(0);
}

/*
运行程序，下面是输出结果的某个段内容
[dendi875@localhost Chapter-11-Threads]$ ./my_rwlock
...
read thread i = 6, tid = 3016452976, counter = 294
read thread i = 5, tid = 3026942832, counter = 294
read thread i = 4, tid = 3037432688, counter = 294
read thread i = 3, tid = 3047922544, counter = 294
read thread i = 7, tid = 3005963120, counter = 294
read thread i = 6, tid = 3016452976, counter = 294
read thread i = 5, tid = 3026942832, counter = 294
read thread i = 4, tid = 3037432688, counter = 294
read thread i = 3, tid = 3047922544, counter = 294
read thread i = 7, tid = 3005963120, counter = 294
wirte thread i = 1, tid = 3068902256, counter = 294, ++counter = 295
wirte thread i = 0, tid = 3079392112, counter = 295, ++counter = 296
wirte thread i = 2, tid = 3058412400, counter = 296, ++counter = 297
read thread i = 6, tid = 3016452976, counter = 297
read thread i = 5, tid = 3026942832, counter = 297
read thread i = 3, tid = 3047922544, counter = 297
read thread i = 7, tid = 3005963120, counter = 297
read thread i = 4, tid = 3037432688, counter = 297
read thread i = 6, tid = 3016452976, counter = 297
read thread i = 5, tid = 3026942832, counter = 297
read thread i = 3, tid = 3047922544, counter = 297
read thread i = 7, tid = 3005963120, counter = 297
read thread i = 4, tid = 3037432688, counter = 297
wirte thread i = 1, tid = 3068902256, counter = 297, ++counter = 298
wirte thread i = 0, tid = 3079392112, counter = 298, ++counter = 299
wirte thread i = 2, tid = 3058412400, counter = 299, ++counter = 300
...
*/