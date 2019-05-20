/**
 * 用ROT-13和异步I/O翻译一个文件
 */

#include "apue.h"
#include <fcntl.h>
#include <aio.h>

#define BSZ 100
#define NBUF 8

/* 定义读写操作状态 */
enum rwop {
    UNUSED = 0, // 未使用的
    READ_PENDING = 1, // 等待读
    WRITE_PENDING = 2 // 等待写
};

struct buf {
    enum rwop       op;
    int             last;       /* 是否到达了文件末尾（0否，1是，全局变量默认值是0）*/
    struct aiocb    aiocb;
    unsigned char   data[BSZ];  /* 读写操作的缓冲区 */
};

struct buf bufs[NBUF];

unsigned char translate(unsigned char c)
{
    if (isalpha(c)) {
        if (c >= 'n') {
            c -= 13;
        } else if (c >= 'a') {
            c += 13;
        } else if (c >= 'N') {
            c -= 13;
        } else {
            c += 13;
        }
    }
    return(c);
}

int main(int argc, char *argv[])
{
    int                 ifd, ofd, i, numop, err, n, j;
    struct stat         sbuf;
    const struct aiocb  *aiolist[NBUF];
    off_t               off = 0;

    if (argc != 3) {
        err_quit("usage：./rot132 infile outfile");
    }
    if ((ifd = open(argv[1], O_RDONLY)) < 0) {
        err_sys("can't open %s", argv[1]);
    }
    if ((ofd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
        err_sys("can't create %s", argv[2]);
    }
    if (fstat(ifd, &sbuf) < 0) {
        err_sys("fstat failed");
    }
    for (i = 0; i < NBUF; i++) {
        bufs[i].op = UNUSED;
        bufs[i].aiocb.aio_buf = bufs[i].data;
        bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE; /* 异步I/O完成后的通知类型 */
        aiolist[i] = NULL;
    }

    numop = 0;
    for (;;) {
        for (i = 0; i < NBUF; i++) {
            switch (bufs[i].op) {
                case UNUSED:
                    /**
                     * 如果还有未读数据，则从输入文件中读取
                     */
                    if (off < sbuf.st_size) {
                        bufs[i].op = READ_PENDING;
                        bufs[i].aiocb.aio_fildes = ifd;
                        bufs[i].aiocb.aio_offset = off;
                        off += BSZ;
                        if (off >= sbuf.st_size) {
                            bufs[i].last = 1;
                        }
                        bufs[i].aiocb.aio_nbytes = BSZ;
                        if (aio_read(&bufs[i].aiocb) < 0) {   /* 将异步读请求写入队列 */
                            err_sys("aio_read failed");
                        }
                        aiolist[i] = &bufs[i].aiocb;
                        numop++;
                    }
                    break;

                case READ_PENDING:
                    /**
                     * 如果异步读AIO还未完成（正在处理中），则跳过检查下一个AIO
                     */
                    if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS) {
                        continue;
                    }
                    /* 如果不是完成，则就是有错误发生 */
                    if (err != 0) {
                        if (err == -1) {
                            err_sys("aio_error failed");
                        } else {
                            err_exit(err, "read failed");
                        }
                    }
                    /* 异步读AIO已经完成，获取它的返回值 */
                    if ((n = aio_return(&bufs[i].aiocb)) < 0) {
                        err_sys("aio_return failed");
                    }
                    if (n != BSZ && !bufs[i].last) {
                        err_quit("short read (%d/%d)", n, BSZ);
                    }
                    /* 翻译缓冲区的内容 */
                    for (j = 0; j < n; j++) {
                        bufs[i].data[j] = translate(bufs[i].data[j]);
                    }

                    bufs[i].op = WRITE_PENDING;
                    bufs[i].aiocb.aio_fildes = ofd;
                    bufs[i].aiocb.aio_nbytes = n;

                    /* 异步写请求入队列 */
                    if (aio_write(&bufs[i].aiocb) < 0) {
                        err_sys("aio_write failed");
                    }
                    break;

                case WRITE_PENDING:
                    /**
                     * 如果异步写操作AIO还未完成（正在处理中），则跳过检查下一个AIO
                     */
                    if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS) {
                        continue;
                    }
                    /* 如果不是完成，则就是有错误发生 */
                    if (err != 0) {
                        if (err == -1) {
                            err_sys("aio_error failed");
                        } else {
                            err_exit(err, "write failed");
                        }
                    }
                    /* 异步写操作完成，将AIO控制块缓冲区标记为为使用 */
                    if ((n = aio_return(&bufs[i].aiocb)) < 0) {
                        err_sys("aio_return failed");
                    }
                    if (n != bufs[i].aiocb.aio_nbytes) {
                        err_quit("short write (%d/%d)", n, BSZ);
                    }
                    aiolist[i] = NULL;
                    bufs[i].op = UNUSED;
                    numop--;
                    break;
            }
        }

        if (numop == 0) {
            if (off >= sbuf.st_size) {
                break;
            }
        } else {
            /* 挂起当前进程，直到观察的AIO控制块有I/O操作完成，则返回0 */
            if (aio_suspend(aiolist, NBUF, NULL) < 0) {
                err_sys("aio_suspend failed");
            }
        }
    }

    bufs[0].aiocb.aio_fildes = ofd;
    if (aio_fsync(O_SYNC, &bufs[0].aiocb) < 0) {
        err_sys("aio_fsync failed");
    }

    exit(0);
}