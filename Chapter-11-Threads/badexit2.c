/**
 * 该程序说明了如果使用线程的栈中的自动变量做为pthread_exit的参数时所产生的问题
 * （自动变量在线程的运行函数返回后就销毁了）
 */

#include "apue.h"
#include <pthread.h>

struct foo {
    int a, b, c, d;
};

void printfoo(const char *s, struct foo *fp)
{
    printf("%s", s);
    printf("  structure at 0x%lx\n", (unsigned long)fp);
    printf("  foo.a = %d\n", fp->a);
    printf("  foo.b = %d\n", fp->b);
    printf("  foo.c = %d\n", fp->c);
    printf("  foo.d = %d\n", fp->d);
}

void *thr_fun1(void *arg)
{
    struct foo foo = {1, 2, 3, 4};

    printfoo("thread 1:\n", &foo);
    pthread_exit((void *)&foo);
}

void *thr_fun2(void *arg)
{
    printf("thread 2：ID is %lu\n", (unsigned long)pthread_self());
    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t   tid1, tid2;
    int         err;
    struct foo *fp;
    void        *retval;

    err = pthread_create(&tid1, NULL, thr_fun1, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread 1");
    }
    err = pthread_join(tid1, (void *)&fp);
    if (err != 0) {
        err_exit(err, "can't join with thread 1");
    }

    sleep(1);

    printf("parent starting second thread\n");

    err = pthread_create(&tid2, NULL, thr_fun2, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread 2");
    }
    err = pthread_join(tid2, &retval);
    if (err != 0) {
        err_exit(err, "can't join with thread 2");
    }
    printf("thread 2 exit code %ld\n", (unsigned long)retval);

    sleep(1);

    printfoo("parent:\n", fp);

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-11-Threads]$ ./badexit2
thread 1:
  structure at 0xb7776370
  foo.a = 1
  foo.b = 2
  foo.c = 3
  foo.d = 4
parent starting second thread
thread 2：ID is 3078056816
thread 2 exit code 0
parent:
  structure at 0xb7776370
  foo.a = 3810798
  foo.b = -1216912508
  foo.c = 4141044
  foo.d = 4133480

*/