/**
 *fstatat()函数实例
 */
#include "apue.h"
#include <fcntl.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *dp;
    int fd;
    char *ptr;
    struct stat buf;

    if (argc != 3) {
        err_quit("usage：my_fstatat.c <directory_name> <file_name>");
    }
    if ((dp = opendir(argv[1])) == NULL) {
        err_sys("can't open directory %s", argv[1]);
    }
    if ((fd = dirfd(dp)) < 0) {
        err_sys("error dirfd");
    }
    if (fstatat(fd, argv[2], &buf, AT_SYMLINK_NOFOLLOW) < 0) {
        err_sys("error fstatat");
    }
    // 加入AT_SYMLINK_NOFOLLOW参数，意思时当文件是软链接时，返回的时软链接本身的信息而不是软链接所
    // 向的源文件信息
    if (S_ISREG(buf.st_mode)) {
        ptr = "regular";
    } else if (S_ISDIR(buf.st_mode)) {
        ptr = "directory";
    } else if (S_ISCHR(buf.st_mode)) {
        ptr = "character device";
    } else if (S_ISBLK(buf.st_mode)) {
        ptr = "block device";
    } else if (S_ISLNK(buf.st_mode)) {
        ptr = "symbolic link";
    } else if (S_ISSOCK(buf.st_mode)) {
        ptr = "socket";
    } else if (S_ISFIFO(buf.st_mode)) {
        ptr = "FIFO";
    } else {
        ptr = "** unknow mode **";
    }

    printf("%s\n", ptr);

    exit(0);
}

/**
 * 做一些实验验证
 * 1.AT_SYMLINK_NOFOLLOW参数的使用
 * [dendi875@localhost ctest]$ mkdir testdir
 * [dendi875@localhost ctest]$ ln -s /etc/passwd /test/ctest/testdir/passwd-so
 * [dendi875@localhost ctest]$ ./my_fstatat testdir passwd-so
 * symbolic link
 * [dendi875@localhost ctest]$ cp -a /etc/passwd /test/ctest/testdir/passwd
 * [dendi875@localhost ctest]$ ./my_fstatat testdir passwd
 * regular
 */