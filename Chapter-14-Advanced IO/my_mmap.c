/**
 * mmap函数实例，使用 mmap能将一个磁盘文件映射到虚拟地址空间的一个缓冲区上，于时，
 * 当从缓冲区取数据时，就相当于读文件中的相应字节
 * 当将数据写入缓冲区时，相应字节就自动写入文件，这样可以在不使用read 和 write的情况下
 * 执行I/O
 */

#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>

#define MAPLEN 20 /* 映射的到虚拟内存的长度 */

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
    // p++;

    close(fd);

    strcpy(p, "hello mmap\n");  /* 向映射的缓冲区中写数据 */

    if (munmap(p, MAPLEN) < 0) {  /* 关闭映射存储区 */
        err_sys("munmap error");
    }

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-14-Advanced IO]$ ./my_mmap temp.mmap
[dendi875@localhost Chapter-14-Advanced IO]$ cat temp.mmap
hello mmap

0、
close(fd);

结论：映射区只要创建成功，文件描述符就可以关闭


1、
打开 p++; 注释
运行
[dendi875@localhost Chapter-14-Advanced IO]$ ./my_mmap temp.mmap
munmap error: Invalid argument

结论：munmap 的入参一定是 mmap 的返回值，不能对 mmap 返回的指针做指针运算

2、
fd = open(argv[1], O_RDONLY | O_CREAT | O_TRUNC, FILE_MODE)
mmap(NULL, MAPLEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
运行
[dendi875@localhost Chapter-14-Advanced IO]$ ./my_mmap temp.mmap
mmap error: Permission denied

结论：创建映射时的权限不能超过 open 时的权限

3、
fd = open(argv[1], O_RDONLY | O_CREAT | O_TRUNC, FILE_MODE)
mmap(NULL, MAPLEN, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)
运行
[dendi875@localhost Chapter-14-Advanced IO]$ ./my_mmap temp.mmap
总线错误 (core dumped)

结论：创建映射时的权限不能超过 open 时的权限

4、
fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE)
mmap(NULL, MAPLEN, PROT_WRITE, MAP_SHARED, fd, 0)
运行
[dendi875@localhost Chapter-14-Advanced IO]$ ./my_mmap temp.mmap
mmap error: Permission denied

结论：创建映射区的过程中，隐含着一次对文件的读操作


5、
mmap(NULL, MAPLEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 10)
运行
[dendi875@localhost Chapter-14-Advanced IO]$ ./my_mmap temp.mmap
mmap error: Invalid argument

结论：文件的偏移量必须是 4k 的整数倍

6、mmap 创建映射区出错的概念非常高，一定要对返回值做检查

*/
