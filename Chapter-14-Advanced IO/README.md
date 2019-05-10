http://cxd2014.github.io/2018/12/09/aio/
https://blog.csdn.net/shreck66/article/details/48765533
https://jin-yang.github.io/post/linux-program-aio.html

struct aiocb {
    int             aio_fildes;
    off_t           aio_offset;
    volatile void   *aio_buf;
    size_t          aio_nbytes;
    int             aio_reqprio;
    struct sigevent aio_sigevent;
    int             aio_lio_opcode;
}

aio_fildes　表示用来打开读或写的文件描述符。
aio_offset  表示读后写操作从aio_offset指定的偏移量开始
对于读操作，数据会复制到缓冲区，该缓冲区从 aio_buf 指定的地址开始
aio_nbytes 表示读或写操作的字节数

aio_sigevent 控制在I/O完成后，如何通知应用程序

struct sigevent {
    int                sigev_notify;
    int                sigev_signo;
    union sigval       sigev_value;
    void (*sigev_notify_function)(union sigval);
    pthread_attr_t     *sigev_notify_attributes;
}

union sigval {
    int  sival_int;
    void *sival_ptr;
};



sigev_notify 控制通知的类型

SIGEV_NONE  异步I/0请求完成后，不通知进程
SIGEV_SIGNAL 异步I/0请求完成后，产生由