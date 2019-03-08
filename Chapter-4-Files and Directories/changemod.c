#include "apue.h"

int main(void)
{
    struct stat buf;

    if (stat("foo", &buf) < 0) {
        err_sys("stat error for foo");
    }
    if (chmod("foo", (buf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
        err_sys("chmod error for foo");
    }
    /* 设置绝对的权限为 rw-r--r-- */
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
        err_sys("chmod error for bar");
    }

    exit(0);
}

/**
 * 原foo文件权限：600
 * 原bar文件权限：666
 *
 * S_IXGRP：八进制：0010 二进制：000 001 000　　所属组执行权限
 *
 * S_ISGID：八进制：02000 二进制 010 000 000
 *
 * S_IRUSR：二进制 100 000 000
 * S_IWUSR：二进制 010 000 000
 * S_IRGRP：二进制 000 100 000
 * S_IROTH：二进制 000 000 100
 *
 * 代码分析：不管文件bar当前权限位如何，都将其权限设置为一个绝对值。
 *110 100 100 相应权限为644 rw-r--r--
 *
 * (buf.st_mode & ~S_IXGRP) | S_ISGID
 *
 * 相对于当前状态设置权限。S_ISGID打开设置组ID位、& ~S_IXGRP关闭组的执行位
 *
 * 实验：
 * [root@localhost ctest]# ll -l bar foo
 * -rw-r--r-- 1 root root 0 3月   7 11:09 bar
 * -rw-rwSrw- 1 root root 0 3月   7 11:09 foo
 */