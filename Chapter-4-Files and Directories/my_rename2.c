/**
 * rename()重命名目录实例；newname不存在
 */

#include "apue.h"

int main(void)
{
    if (rename("a.dir", "b.dir") < 0) {
        err_sys("rename error");
    }
    printf("a.dir renamed b.dir\n");

    exit(0);
}

/**
 * 实验：
[root@localhost Chapter-4-Files and Directories]# mkdir a.dir
[root@localhost Chapter-4-Files and Directories]# ./my_rename2
a.dir renamed b.dir
 */