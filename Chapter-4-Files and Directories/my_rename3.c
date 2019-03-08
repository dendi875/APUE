/**
 * rename()对文件重命名，newname已经存在。要求：
 * 1.newname只能是文件不能是目录
 * 2.调用进程对oldname所在目录和newname所在目录必须有写权限
 * 重命名为已经存在的名字，先把newname删除然后将oldname重命名为newname
 */

#include "apue.h"

int main(void)
{
    if (rename("c.file", "cc.file") < 0) {
        err_sys("rename error");
    }
    printf("c.file renamed cc.file\n");

    exit(0);
}


/**
 * 实验：
[root@localhost Chapter-4-Files and Directories]# echo 'c' > c.file
[root@localhost Chapter-4-Files and Directories]# echo 'cc' > cc.file
[root@localhost Chapter-4-Files and Directories]# ./my_rename3
c.file renamed cc.file
[root@localhost Chapter-4-Files and Directories]# cat cc.file
c
 */


