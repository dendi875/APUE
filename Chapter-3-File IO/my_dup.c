/**
 * dump函数使用实例
 */

#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) // 0666

int main(int argc, char *argv[])
{
    int oldfd, newfd;
    off_t oldpos, newpos;
    int old_status_flag, new_status_flag;
    struct stat oldbuf, newbuf;

    umask(0);
    if ((oldfd = open("access.log", O_RDWR | O_CREAT, RWRWRW)) < 0) {
        err_sys("error open");
    }
    if ((newfd = dup(oldfd)) < 0) {
        err_sys("error dup");
    }
    if ((old_status_flag = fcntl(oldfd, F_GETFL, 0)) < 0) {
        err_sys("fcntl error for fd %d", oldfd);
    }
    if ((new_status_flag = fcntl(newfd, F_GETFL, 0)) < 0) {
        err_sys("fcntl error for fd %d", newfd);
    }
    // 验证新老两个文件描述符他们的文件状态标志、文件偏移量、inode号是否一样
    if (old_status_flag == new_status_flag) {
        err_msg("file status flags is the same.\n");
    }

    if ((oldpos = lseek(oldfd, 0, SEEK_CUR)) < 0 || (newpos = lseek(newfd, 0, SEEK_CUR)) < 0) {
        err_sys("lseek error");
    }
    if (oldpos == newpos) {
        err_msg("file offset is the same.\n");
    }

    if (fstat(oldfd, &oldbuf) < 0 || (fstat(newfd, &newbuf)) < 0) {
        err_sys("fstat error");
    }
    if (oldbuf.st_ino == newbuf.st_ino) {
        err_msg("file inode number（%d） is the same.\n", oldbuf.st_ino);
    }

    close(oldfd);
    close(newfd);

    exit(0);
}

/**
 *dump 复制一个文件描述符，返回新的文件描述符一定是当前可用文件描述符中最小的数值。
 *很可能返回的值为3，因为0，1，2被当前的shell进程占用了。
 *
 * 返回的新文件描述符和老的描述符它们指向同一文件表项，所以它们共享文件状态标志，同一文件偏移量
 * 同个inode
 */