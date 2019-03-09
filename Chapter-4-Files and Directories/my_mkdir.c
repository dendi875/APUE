/**
 * mkdir函数使用实例
 */

#include "apue.h"

int main(void)
{
    if (mkdir("/test/ctest/dir1", DIR_MODE) < 0) {
        err_sys("mkdir error");
    }
    printf("created success\n");

    exit(EXIT_SUCCESS);
}

/*实验：
[root@localhost Chapter-4-Files and Directories]# ./my_mkdir
created success
[root@localhost Chapter-4-Files and Directories]# ls -ld /test/ctest/dir1/
drwxr-xr-x 2 root root 4096 3月   9 17:50 /test/ctest/dir1/

注意：新创建的权限为(mode & ~umask & 0777)，man 2 mkdir有说明*/