/**
 * 当异步I/O完成后，通过信号来通知
 */

#include "apue.h"
#include <fcntl.h>
#include <aio.h>
#include <signal.h>

#define BUFFER_SIZE 4096

void aio_completion_handler(int signo, siginfo_t *info, void *context)
{
    int n;
    struct aiocb *praiocb;

    if (info->si_signo == SIGIO) {   /* 确保这是我们自己的信号 */
        praiocb = info->si_value.sival_ptr;
        /* 判断请求是否完成 */
        if (aio_error(praiocb) == 0) {
            /* 获取返回值 */
            n = aio_return(praiocb);
            printf("n = %d\n", n);
        }
    }
}

int main(void)
{
    int fd;
    struct aiocb raiocb;
    struct sigaction act;

    if ((fd = open("temp.file", O_RDONLY)) < 0) {
        err_sys("can't open temp.file");
    }

    /* 注册信号处理动作 */
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = aio_completion_handler;
    if (sigaction(SIGIO, &act, NULL) < 0) {
        err_sys("sigaction(SIGIO) error");
    }

    memset(&raiocb, 0, sizeof(struct aiocb));   /* 将结构体置为0 */
    /* 初始化aiocb结构体 */
    raiocb.aio_fildes = fd;
    raiocb.aio_buf = malloc(BUFFER_SIZE + 1);
    raiocb.aio_nbytes = BUFFER_SIZE;
    raiocb.aio_offset = 0;

    /**
     * 设置当异步I/0完成后的通知方式为信号处理
     */
    raiocb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    raiocb.aio_sigevent.sigev_signo = SIGIO;
    raiocb.aio_sigevent.sigev_value.sival_ptr = &raiocb;

    /* 把异步读请求写入队列 */
    if (aio_read(&raiocb) < 0) {
        err_sys("aio_read failed");
    }

    printf("异步读开始\n");
    sleep(2);
    printf("异步读结束\n");

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-14-Advanced IO]$ ./my_aio_notify_signal
异步读开始
n = 4096
异步读结束
*/