/**
 * 多线程模拟如果不对共享资源加锁会造成的后果
 */

#include "apue.h"
#include <pthread.h>

int globalvar;

void *thr_fun1(void *arg)
{
    int i, j;

    for (i = 0; i < 20; i++) {
        j = globalvar;
        j = j + 1;
        printf(".");
        fflush(stdout);
        sleep(1);
        globalvar = j;
    }

    return NULL;
}

int main(void)
{
    pthread_t   tid1;
    int         err;
    int         i;

    err = pthread_create(&tid1, NULL, thr_fun1, NULL);
    if (err != 0) {
        err_exit(err, "can't create thread 1");
    }
    for (i = 0; i < 20; i++) {
        globalvar = globalvar + 1;
        printf("o");
        fflush(stdout);
        sleep(1);
    }

    err = pthread_join(tid1, NULL);
    if (err != 0) {
        err_exit(err, "can't join with thread 1");
    }

    printf("\nglobalvar = %d\n", globalvar);

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-11-Threads]$ time ./thread2
o.o.o.o.o.o..oo.o.o.o.o..o.oo.o..o.oo.o.
globalvar = 21

real    0m20.014s
user    0m0.001s
sys     0m0.002s

参考资料：
https://www.ibm.com/developerworks/cn/linux/thread/posix_thread1/index.html
https://www.cnblogs.com/mywolrd/archive/2009/02/05/1930707.html#ref
*/