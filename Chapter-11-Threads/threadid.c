/**
 *打印线程ID程序
 */
#include "apue.h"
#include <pthread.h>

pthread_t ntid;

void printids(const char *s)
{
    pid_t       pid;
    pthread_t   tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid,
        (unsigned long)tid);
}

void *thr_func(void *arg)
{
    printids("new thread：");
    return((void *)0);
}

int main(void)
{
    int err;

    err = pthread_create(&ntid, NULL, thr_func, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread");
    }
    printids("main thread：");
    sleep(1);
    exit(0);
}



/*int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                  void *(*start_routine) (void *), void *arg);

pthread_create创建一个新的线程，新的线程ID保存在thread指针指向的对象中。
attr指针用于定制线程的属性，为NULL时用默认的线程属性，新创建的线程从start_routine函数的地址开始
运行，该函数只有一个无类型的指针参数arg。如果需要向start_routine函数传递的参数有一个以上，那么需要
把这些参数放到一个结构体中，然后把这个结构体的地址作为arg参数传入。

这个程序有两个竞争条件：
1、主线程和新线程之间的竞争。如果主线程不休眠，那么有可能主线程运行后就退出了从而整个进程就已经终止了，这时新的线程可能还没有机会运行。
2、新线程的运行和主线程调用pthread_create的返回之间的竞争。如果新线程在主线程调用pthread_create返回
之前就已经运行了，那么新线程看到的ntid的内容是未初始的值。所以新线程不能安全的使用ntid来获取自己的
线程ID而是用pthread_self来获取。

gcc时要加上-pthread选项

实验：
[dendi875@localhost Chapter-11-Threads]$ ./threadid
main thread： pid 6493 tid 3077936832 (0xb77596c0)
new thread： pid 6493 tid 3077933936 (0xb7758b70)
*/