/**
 * readlinkat函数使用实例
 */

#include "apue.h"
#include <dirent.h>

#define BUFFER_SIZE 10

int main(int argc, char *argv[])
{
    DIR *dp;
    int fd;
    char buf[BUFFER_SIZE + 1];

    if (argc != 3) {
        err_quit("usage：my_readlinkat <dirname> <symbolic link>");
    }
    if ((dp = opendir(argv[1])) == NULL) {
        err_sys("opendir error for %s", argv[1]);
    }
    if ((fd = dirfd(dp)) < 0) {
        err_sys("dirfd error");
    }
    if (readlinkat(fd, argv[2], buf, BUFFER_SIZE) < 0) {
        err_sys("readlinkat error");
    }
    buf[BUFFER_SIZE] = '\0';
    closedir(dp);

    printf("buf = %s\n", buf);

    exit(0);
}

/*实验：
[root@localhost ctest]# echo "0123456789" > testfile
[root@localhost ctest]# mkdir -p testdir1/testdir11
[root@localhost ctest]# ln -s ../../testfile testdir1/testdir11/
[root@localhost ctest]# ll testdir1/testdir11/testfile
lrwxrwxrwx 1 root root 14 3月   9 14:55 testdir1/testdir11/testfile -> ../../testfile

[root@localhost ctest]# ./my_readlinkat testdir1/testdir11/ testfile
buf = ../../test

注意：readlink和readlinkat函数在buf返回的符号链接的内容中不以null（'\0'）字节终止。
*/