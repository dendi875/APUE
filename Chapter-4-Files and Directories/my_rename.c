/**
 * 使用rename()对文件重命名（newname不存在）
 */

#include "apue.h"

int main(void)
{
    if (rename("a.file", "b.file") < 0) {
        err_sys("rename error");
    }
    printf("a.file renamed b.file\n");

    exit(0);
}

/**
 * 实验：
[root@localhost Chapter-4-Files and Directories]# touch a.file
[root@localhost Chapter-4-Files and Directories]# ./my_rename
a.file renamed b.file
 */