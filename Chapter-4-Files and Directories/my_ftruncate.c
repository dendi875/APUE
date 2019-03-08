/**
 * 1.用open打开一个文件
 * 2.用write函数写10个字符
 * 3.用stat函数获取st_size成员看是否为10
 * 4.用ftruncate函数截断为5个字节
 */

#include "apue.h"
#include <fcntl.h>

#define LENGTH 5

int main(void)
{
    int fd;
    char buf[] = "abcdefghij";
    struct stat sb1, sb2;
    if ((fd = open("testfile", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
        err_sys("open error");
    }
    if (write(fd, buf, 10) != 10) {
        err_sys("write buf error");
    }
    if (fstat(fd, &sb1) < 0) {
        err_sys("fstat error");
    }
    printf("File size：%lld bytes\n", (long long) sb1.st_size);

    if (ftruncate(fd, LENGTH) < 0) {
        err_sys("ftruncate error");
    }
    if (fstat(fd, &sb2) < 0) {
        err_sys("fstat error");
    }
    printf("File size：%lld bytes\n", (long long) sb2.st_size);

    exit(EXIT_SUCCESS);
}

/**
 * 实验：
 *[root@localhost Chapter-4-Files and Directories]# ./my_stat2
 *File size：10 bytes
 *File size：5 bytes
 */