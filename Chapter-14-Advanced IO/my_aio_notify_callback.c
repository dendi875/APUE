/**
 * 通过回调函数来通知异步 I/O
 */

#include "apue.h"
#include <fcntl.h>
#include <aio.h>

#define BUFFER_SIZE 4096

void aio_completion_handler(union sigval arg)
{
    struct aiocb *praiocb;  /* 用来获取读aiocb结构体的指针 */
    int n;

    praiocb = (struct aiocb *)arg.sival_ptr;

    /* 判断请求是否完成 */
    if (aio_error(praiocb) == 0) {
        /* 获取返回值 */
        n = aio_return(praiocb);
        printf("n = %d\n", n);
    }
}

int main(void)
{
    int fd;
    struct aiocb raiocb;

    if ((fd = open("temp.file", O_RDONLY)) < 0) {
        err_sys("can't open temp.file");
    }

    memset(&raiocb, 0, sizeof(struct aiocb));   /* 将结构体置为0 */

    /* 初始化raiocb结构体 */
    raiocb.aio_fildes = fd;
    raiocb.aio_buf = malloc(BUFFER_SIZE + 1);
    raiocb.aio_nbytes = BUFFER_SIZE;
    raiocb.aio_offset = 0;

    /* 设置当异步I/O完成后的通知方式 */
    raiocb.aio_sigevent.sigev_notify = SIGEV_THREAD;    /* 使用线程回调通知 */
    raiocb.aio_sigevent.sigev_notify_function = aio_completion_handler; /* 设置回调函数 */
    raiocb.aio_sigevent.sigev_notify_attributes = NULL; /* 使用线程默认属性 */
    raiocb.aio_sigevent.sigev_value.sival_ptr = &raiocb; /* 在aiocb控制块中加入自己的引用 */

    /* 异步读请求写入队列 */
    if ((aio_read(&raiocb)) < 0) {
        err_sys("aio_read failed");
    }

    printf("异步读开始\n");
    sleep(2);
    printf("异步读结束\n");

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-14-Advanced IO]$ ./my_aio_notify_callback
异步读开始
n = 4096
异步读结束
*/