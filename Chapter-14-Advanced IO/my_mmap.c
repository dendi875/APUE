/**
 * mmap函数实例，使用 mmap能将一个磁盘文件映射到虚拟地址空间的一个缓冲区上，于时，
 * 当从缓冲区取数据时，就相当于读文件中的相应字节
 * 当将数据写入缓冲区时，相应字节就自动写入文件，这样可以在不使用read 和 write的情况下
 * 执行I/O
 */

#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>

#define MAPLEN 4096 /* 映射的到虚拟内存的长度，必须是内存面的整数倍 */

int main(int argc, char *argv[])
{
    int             fd;
    char            *p;

    if (argc != 2) {
        fprintf(stderr, "usage：%s filename\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
        err_sys("open error");
    }

    if (ftruncate(fd, MAPLEN) < 0) {
        err_sys("ftruncate error");
    }

    p = mmap(NULL, MAPLEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        err_sys("mmap error");
    }

    strcpy(p, "hello mmap\n");  /* 向映射的缓冲区中写数据 */

    munmap(p, MAPLEN);          /* 关闭映射存储区 */

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-14-Advanced IO]$ cat temp.mmap
hello mmap
*/
