/**
 * 该程序用来测试进程默认最多能创建线程的最大数量
 */

#include "apue.h"
#include <pthread.h>

void *thr_fun(void *arg)
{
    while (1) {
        sleep(1);
    }
    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t       tid;
    int             err;
    int             i = 0;

    while (1) {
        err = pthread_create(&tid, NULL, thr_fun, NULL);
        if (err != 0) {
            err_exit(err, "can't create thread");
        }
        printf("%d\n", ++i);
    }
    exit(0);
}
