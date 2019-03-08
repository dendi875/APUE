/**
 * rename对目录重命名，newname已经存在。要求：
 * 1.newname必须是一个目录
 * 2.newname必须是一个空目录（目录中只有.和..）
 * 如果重命令后的目录存在，则删除newname然后将oldname重命名为newname
 */

#include "apue.h"

int main(void)
{
    if (rename("c.dir", "cc.dir") < 0) {
        err_sys("rename error");
    }

    printf("c.dir renamed cc.dir\n");
    exit(0);
}

/**
 * 实验：
[root@localhost tmp]# mkdir {c.dir,cc.dir} && touch c.dir/c.file

[root@localhost tmp]# ./my_rename4
c.dir renamed cc.dir
[root@localhost tmp]# ls -l c.dir cc.dir/
ls: 无法访问c.dir: 没有那个文件或目录
cc.dir/:
总用量 0
-rw-r--r-- 1 root root 0 3月   8 20:48 c.file
 */