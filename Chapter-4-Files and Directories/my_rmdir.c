/**
 * rmdir函数使用实例
 */

#include "apue.h"

int main(int argc, char *argv[])
{
    struct stat statbuf;

    if (argc != 2) {
        err_quit("usage：my_rmdir <dirname>");
    }
    if (lstat(argv[1], &statbuf) < 0) {
        err_sys("lstat error");
    }
    if (!S_ISDIR(statbuf.st_mode)) {
        err_quit("%s not a directory", argv[1]);
    }
    printf("links number = %d\n", statbuf.st_nlink); /* 打印目录的链接计数 */

    if (rmdir(argv[1]) < 0) {
        err_sys("rmdir error");
    }
    printf("deleted success\n");

    exit(0);
}


/*实验：
[root@localhost Chapter-4-Files and Directories]# ./my_rmdir /test/ctest/dir1/
links number = 2
deleted success
[root@localhost Chapter-4-Files and Directories]# ll /test/ctest/dir1
ls: 无法访问/test/ctest/dir1: 没有那个文件或目录

注意：
1.rmdir函数用于删除一个空目录（只包含.和..这两项的目录）
2.任何一个空目录都有两个链接计数（一个是.和目录名本身）
3.每在父目录中创建一个目录都能使父目录的链接计数增加1*/