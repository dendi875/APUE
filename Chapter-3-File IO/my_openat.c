/**
 * 书上没openat()函数实例，自己写的实例
 */
#include "apue.h"
#include <dirent.h>
#include <fcntl.h>

#define RWRR (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) // 0644
#define CHAR_NUM   5

int main(int argc, char *argv[])
{
    DIR *dp;
    int dfd;
    int fd;
    int n;

    if (argc != 3) {
        err_quit("usage：my_openat <directory_name> <file_name>");
    }
    if ((dp = opendir(argv[1])) == NULL) {
        err_sys("can't open %s", argv[1]);
    }
    if ((dfd = dirfd(dp)) < 0) {
        closedir(dp);
        err_sys("dirfd error");
    }

    umask(0);
    if ((fd = openat(dfd, argv[2], O_RDWR | O_CREAT | O_TRUNC, RWRR)) < 0) {
        closedir(dp);
        err_sys("openat error for file_name %s：", argv[2]);
    }
    // 向文件描述符中写内容
    if ((n = write(fd, "hello", CHAR_NUM)) != CHAR_NUM) {
        close(fd);
        closedir(dp);
        err_sys("write error");
    }

    close(fd);
    closedir(dp);

    exit(0);
}

/**
 * 通过man 2 openat知道int openat(int dirfd, const char *pathname, int flags, mode_t mode);
 * 第一个参数dirfd是需要一个目录的描述符，这里就需要两个函数：
 * opendir()打开一个目录返回DIR *类型指针，如果出错返回NULL指针（空指针）
 * dirfd()获取一个目录的文件描述符，如果出错返回-1
 *
 * 如果openat中第三个参数有O_CREAT那么必须指定第四个参数（告诉系统你创建的文件是什么权限）
 *
 * 运行实例：
 * [root@localhost Chapter-3-File IO]# ./my_openat open log.txt
 */