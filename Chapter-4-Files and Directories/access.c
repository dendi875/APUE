#include "apue.h"
#include <fcntl.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        err_ret("usage：my_access <pathname>");
    }
    if (access(argv[1], R_OK) < 0) {
        err_ret("access error for %s", argv[1]);
    } else {
        printf("read access OK\n");
    }

    if (open(argv[1], O_RDONLY) < 0) {
        err_ret("open error for %s", argv[1]);
    } else {
        printf("open for reading OK\n");
    }

    exit(0);
}

/**
 * R_OK测试文件的读权限
 *
 * open()是以进程的有效用户ID和有效组ID为基础来测试访问权限。
 * access()是以进程的实际用户ID和实际组ID为基础来测试访问权限。
 *
 * geteuid() 获取进程有效用户ID
 * getegid() 获取进程有效组ID
 *
 * 实验：
 *[dendi875@localhost Chapter-4-Files and Directories]$ ./access access
 *read access OK
 *open for reading OK
 *
 *[dendi875@localhost Chapter-4-Files and Directories]$ ./access /etc/shadow
 *access error for /etc/shadow: Permission denied
 *open error for /etc/shadow: Permission denied
 */