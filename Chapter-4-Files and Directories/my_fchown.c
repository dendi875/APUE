/**
 * fchown()修改文件所有者示例
 */

#include "apue.h"
#include <fcntl.h>
#include <pwd.h>

#define USER_NAME "zq"

int main(int argc, char *argv[])
{
    int fd;
    struct stat oldbuf, newbuf;
    struct passwd *ptr;

    if (argc != 2) {
        err_quit("usage：my_chown <filename>");
    }
    printf("进程的有效用户ID为：%d\n", getuid());

    if ((fd = open(argv[1], O_RDONLY | O_CREAT, FILE_MODE)) < 0) {
        err_sys("error open %s", argv[1]);
    }
    if (fstat(fd, &oldbuf) < 0) {
        err_sys("error fstat");
    }

    // 先打印文件老的uid
    printf("%s 文件老的uid为：%d\n", argv[1], oldbuf.st_uid);

    // 使用getpwnam函数获取zq用户的uid，然后用fchown函数修改fd文件描述符的uid为zq用户的uid
    if ((ptr = getpwnam(USER_NAME)) == NULL) {
        err_sys("error getpwnam");
    } else {
        printf("zq用户的uid为：%d\n", ptr->pw_uid);
    }

    if (fchown(fd, ptr->pw_uid, -1) < 0) {
        err_sys("error fchown");
    } else {
        // 重新获取fd文件描述符的uid
        if (fstat(fd, &newbuf) < 0) {
            err_sys("error fstat");
        }
    }
    // 打印fd新的uid和老的对照下
    printf("%s 文件新的uid为：%d\n", argv[1], newbuf.st_uid);

    // 关闭打开的文件
    close(fd);
    endpwent();

    exit(0);
}

/**
 * 用open或creat创建新文件时，新文件的用户ID为进程的有效用户ID.
 *
 * 实验：
 *  [dendi875@localhost ctest]$ ll my_fchown
 *  -rwxrwxr-x 1 dendi875 dendi875 7546 3月   7 20:53 my_fchown
    [root@localhost ctest]# ./my_fchown testfile
    进程的有效用户ID为：0
    testfile 文件老的uid为：0
    zq用户的uid为：505
    testfile 文件新的uid为：505
    [root@localhost ctest]# ll testfile
    -rw-r--r-- 1 zq root 0 3月   7 21:05 testfile
 *
 *
 */