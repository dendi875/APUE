/**
 * futimens函数使用实例
 */

#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int             i, fd;
    struct stat     statbuf;
    struct timespec times[2];

    for (i = 1; i < argc; i++) {
        if (stat(argv[i], &statbuf) < 0) {
            err_ret("%s：stat error", argv[i]);
            continue;
        }
        if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) {
            err_ret("%s：open error", argv[i]);
            continue;
        }
        times[0] = statbuf.st_atim;
        times[1] = statbuf.st_mtim;
        if (futimens(fd, times) < 0) {
            err_ret("%s：futimens error", argv[i]);
        }
        close(fd);
    }

    exit(EXIT_SUCCESS);
}

/*实验：
查看文件的最后访问时间
[root@localhost ctest]# ls -lu a.txt testfile
-rw-r--r-- 1 dendi875 root 0 3月   9 15:45 a.txt
-rw-r--r-- 1 root     root 0 3月   9 14:51 testfile


查看文件的最后修改时间
[root@localhost ctest]# ls -l a.txt testfile
-rw-r--r-- 1 dendi875 root 0 3月   9 15:45 a.txt
-rw-r--r-- 1 root     root 0 3月   9 14:51 testfile

查看文件状态最后修改时间
[root@localhost ctest]# ls -lc a.txt testfile
-rw-r--r-- 1 dendi875 root 0 3月   9 17:22 a.txt
-rw-r--r-- 1 root     root 0 3月   9 17:22 testfile

打印现在时间
[root@localhost ctest]# date
2019年 03月 09日 星期六 17:26:20 CS

运行程序
[root@localhost ctest]# ./futimens a.txt testfile

查看文件的最后访问时间
[root@localhost ctest]# ls -lu a.txt testfile
-rw-r--r-- 1 dendi875 root 0 3月   9 15:45 a.txt
-rw-r--r-- 1 root     root 0 3月   9 14:51 testfile

查看文件的最后修改时间
[root@localhost ctest]# ls -l a.txt testfile
-rw-r--r-- 1 dendi875 root 0 3月   9 15:45 a.txt
-rw-r--r-- 1 root     root 0 3月   9 14:51 testfile

最后文件状态修改时间
[root@localhost ctest]# ls -lc a.txt testfile
-rw-r--r-- 1 dendi875 root 0 3月   9 17:27 a.txt
-rw-r--r-- 1 root     root 0 3月   9 17:27 testfile
*/
