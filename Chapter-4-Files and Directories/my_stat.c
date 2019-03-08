/**
 * stat()函数实例，获取文件所有者ID，组ID
 */

#include "apue.h"

int main(int argc, char *argv[])
{
    struct stat buf;
    uid_t uid;
    gid_t gid;

    if (argc != 2) {
        err_quit("usage：my_stat <filename>");
    }
    if (stat(argv[1], &buf) < 0) {
        err_sys("error stat");
    }
    uid = buf.st_uid;
    gid = buf.st_gid;

    printf("uid = %d, gid = %d\n", uid, gid);

    exit(0);
}

/**
 * 实验：
 *
 * [root@localhost Chapter-4-Files and Directories]# ./my_stat my_stat
 * uid = 0, gid = 0
 */