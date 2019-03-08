/**
 * 列出一个目录下所有文件
 */
#include "apue.h"
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR             *dp;
    struct dirent   *dirp;

    if (argc != 2) {
        err_quit("usage：ls directory_name");
    }

    if ((dp = opendir(argv[1])) == NULL) {
        err_sys("can't open %s", argv[1]);
    }
    while ((dirp = readdir(dp)) != NULL) {
        printf("%s\n", dirp->d_name);
    }

    closedir(dp);

    exit(0);
}

/**
 * 实验：
 * [dendi875@localhost Chapter-1-System Overview]$ ./ls /etc/pki/CA/private/
 * can't open /etc/pki/CA/private/: Permission denied
 *
 * [root@localhost Chapter-1-System Overview]# ./ls /dev/tty
 * can't open /dev/tty: Not a directory
 */