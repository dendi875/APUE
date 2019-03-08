/**
 * 书上没openat()函数实例，自己写的实例
 */
#include "apue.h"
#include <fcntl.h>

#define CHAR_NUM   5

int main(int argc, char *argv[])
{
    int fd;
    int n;

    if (argc != 2) {
        err_quit("usage：my_openat2 <file_name>");
    }
    umask(0);
    if ((fd = openat(AT_FDCWD, argv[1], O_RDWR | O_CREAT | O_APPEND, FILE_MODE)) < 0) {
        err_sys("openat error for file_name %s：", argv[1]);
    }
    // 向文件描述符中写内容
    if ((n = write(fd, "world", CHAR_NUM)) != CHAR_NUM) {
        close(fd);
        err_sys("write error");
    }
    close(fd);

    exit(0);
}

/**
 * 如果openat第一个参数是AT_FDCWD，第二参数指定了相对路径，那么第二个参数文件名是相对于
 * 当前工作目录获取。
 *
 * 运行实例：
 * [dendi875@localhost Chapter-3-File IO]$ ./my_openat2 log2.txt
 */