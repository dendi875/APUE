/**
 *sigwait可以把异步产生的信号用同步方式处理
 *
 *使用了互斥量+条件变量
 */

#include "apue.h"
#include <pthread.h>

int         quitflag;   /* 全局变量 */
sigset_t    mask;   /* 信号集，全局变量多个线程共享 */

pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER; /* 定义互斥量并初始化 */
pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;   /* 定义条件变量并初始化 */

/**
 * 专门对信号进行处理的线程
 *
 * sigwait函数是分三步处理（原子操作）
 * 1、函数刚执行时解除对信号的阻塞
 * 2、函数调用过程中挂起线程，直到等待的信号（解除的那些信号）到达
 * 3、函数返回之前会对第1步解除的信号进行阻塞
 */
void *thr_fun(void *arg)
{
    int err, signo;

    for (;;) {
        err = sigwait(&mask, &signo);
        if (err != 0) {
            err_exit(err, "sigwait failed");
        }
        switch (signo) {
            case SIGINT:
                printf("\ninterrupt\n");
                break;

            case SIGQUIT:
                pthread_mutex_lock(&mutex); /* 对互斥量进行加锁 */
                quitflag = 1;               /* 操作全局资源变量 */
                pthread_mutex_unlock(&mutex);   /* 对互斥量进行解锁 */
                /* 发送通知，唤醒等待该条件变量的那些线程，告诉它们条件已经满足了 */
                pthread_cond_signal(&cond);
                return((void *)0);

            default :
                printf("unexpected signal %d\n", signo);
                exit(1);
        }
    }
}

int main(void)
{
    int err;
    sigset_t        oldmask;
    pthread_t       tid;

    sigemptyset(&mask);  /* 构造我们自己的信号集 */
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    /* 使用构造好的信号集+pthread_sigmask函数来设置线程的阻塞信号集 */
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0) {
        err_exit(err, "SIG_BLOCK error");
    }

    /* 创建线程 */
    if ((err = pthread_create(&tid, NULL, thr_fun, NULL)) != 0) {
        err_exit(err, "can't create thread");
    }

    pthread_mutex_lock(&mutex); /* 对互斥量进行加锁 */
    while (quitflag == 0) {     /* 只要条件不满足就等待其它线程广播 */
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    /**
     * SIGQUIT已经被捕捉到，然后sigwait已经返回，这时会重新对SIGINI，SIGQUIT进行阻塞
     * 所以要把信号阻塞集状态恢复到对SIGINI，SIGQUIT进行阻塞之前，
     * 这里为什么要使用sigprocmask而不是pthread_sigmask？？
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        err_sys("SIG_SETMASK error");
    }

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-12-Thread Control]$ ./suspend
^C
interrupt
^C
interrupt
^C
interrupt
^\[dendi875@localhost Chapter-12-Thread Control]$


本程序中条件满足指的是thr_fun线程接收到SIGQUIT信号并把quitflag变量设置为1
*/