/**
 * 异步读操作 aio_read 示例
 */

#include "apue.h"
#include <fcntl.h>
#include <aio.h>

#define BUFFER_SIZE 8192    /* 8k */

int main(void)
{
    int fd, n;
    struct aiocb raiocb;
    char *buffer;

    if ((fd = open("temp.file", O_RDONLY)) < 0) {
        err_sys("can't open temp.file");
    }

    buffer = malloc(BUFFER_SIZE);       /* 开辟一块内存空间 */
    memset(&raiocb, 0, sizeof(struct aiocb));     /* 将结构体清零 */
    raiocb.aio_fildes = fd;   /* 从哪里读 */
    raiocb.aio_buf = buffer;  /* 读到的数据存在哪里 */
    raiocb.aio_nbytes = BUFFER_SIZE; /* 每次读多少个字节 */
    raiocb.aio_offset = 0;    /* 读文件描述符的偏移量从哪里开始 */

    /* 进行异步读请求入队列 */
    if (aio_read(&raiocb) < 0) {
        err_sys("aio_read failed");
    }
    printf("Request enqueued!\n");

    /* 循环等待直到异步读操作完成 */
    while (aio_error(&raiocb) == EINPROGRESS) {
        printf("Working...\n");
    }

    /* 异步操作完成后，获取异步操作的返回值 */
    if ((n = aio_return(&raiocb)) < 0) {
        err_sys("aio_return failed");
    }

    printf("n = %d\n", n);

    free(buffer);
    close(fd);

    exit(0);
}

/*
实验
编译：
[dendi875@localhost Chapter-14-Advanced IO]$ gcc -I../ -lrt my_aio_read.c -o my_aio_read
*/