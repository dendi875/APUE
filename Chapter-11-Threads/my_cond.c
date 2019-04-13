/**
 * 利用条件变量实现一个经典的生产者-消费者模型
 */

#include "apue.h"
#include <pthread.h>

/* 链表作为公用数据，需要互斥量进行保护 */
struct msg {
    int num;
    struct msg *next;
};

struct msg *head = NULL;   /* 永远指向链表的第一个节点 */
struct msg *mp = NULL;     /* 用来指向动态分配的节点 */

/* 初始化条件变量和互斥量 */
pthread_mutex_t    mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t     cond = PTHREAD_COND_INITIALIZER;

/**
 * 链表消费者线程
 *
 * 链表中有节点就删除链表的头节点，如果链表恰好是空的就等待链表中有节点的时候再删除
 */
void *consumer(void *arg)
{
    for (;;) {
        pthread_mutex_lock(&mutex); /* 锁住互斥量（为了使用链表这个共享数据）*/
        /**
        * 如果恰好链表是空的，那么需要调用pthread_cond_wait阻塞等待发送信号
        * 函数调用过程如下：
        * 1、立即解锁互斥量，因为不解锁其它线程（比如生产者线程）无法获得锁对链表进行添加节点或修改
        * 2、阻塞当前线程等待着其它线程发送条件满足的信号给我们
        * 3、当收到信号被唤醒时该函数并不立即返回，而是重新锁住互斥量后再返回，如果不锁住下面对链表进
        * 行操作有可能就会造成数据混乱
        */
        while (head == NULL) {
            pthread_cond_wait(&cond, &mutex);
        }
        /* 现在互斥量是锁住状态，可以操作共享数据了 */
        mp = head;
        head = mp->next;
        pthread_mutex_unlock(&mutex); /* 解锁互斥量 */

        printf("consumer：%d\n", mp->num);
        free(mp);
        sleep(rand() % 5);
    }
}

/**
 * 链表生产者线程
 */
void *producter(void *arg)
{
    for (;;) {
        mp = malloc(sizeof(struct msg));
        mp->num = rand() % 1000 + 1;
        printf("producter：%d\n", mp->num);

        pthread_mutex_lock(&mutex);   /* 锁住互斥量 */
        mp->next = head;
        head = mp;
        pthread_mutex_unlock(&mutex);   /* 释放互斥量 */

        /**
         * 条件已经满足（链表中有节点了），发送信号唤醒那些等待该条件的线程
         */
        pthread_cond_signal(&cond);

        sleep(rand () % 5);
    }
}

/**
 * 主控线程，负责创建线程，回收线程
 */
int main(void)
{
    pthread_t       ptid, ctid;
    int             err;

    srand(time(NULL));

    err = pthread_create(&ptid, NULL, producter, NULL);
    if (err != 0) {
        err_exit(err, "can't create producter thread");
    }
    err = pthread_create(&ctid, NULL, consumer, NULL);
    if (err != 0) {
        err_exit(err, "can't create consumer thread");
    }

    err = pthread_join(ptid, NULL);
    if (err != 0) {
        err_exit(err, "can't join with producter thread");
    }
    err = pthread_join(ctid, NULL);
    if (err != 0) {
        err_exit(err, "can't join with producter thread");
    }

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-11-Threads]$ ./my_cond
producter：186
producter：566
producter：636
consumer：636
consumer：566
producter：763
producter：682
consumer：682
consumer：763
producter：608
consumer：608
consumer：186
producter：206
producter：387
producter：142
consumer：142
consumer：387
producter：514
consumer：514
producter：496
producter：822
producter：80
consumer：80
producter：772
consumer：772
consumer：822
consumer：496
producter：53
consumer：53
producter：597
producter：246
^C

参考资料：
https://www.ibm.com/developerworks/cn/linux/thread/posix_thread3/index.html
*/