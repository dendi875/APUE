/**
 * 使用fstat()函数做一些验证：
 * 通常进程的有效用户ID等于进程的实际用户ID，进程的有效组ID等于进程的实际组ID
 */

#include "apue.h"

int main(void)
{
    struct stat buf;
    uid_t uid, euid;
    gid_t gid, egid;

    if (fstat(STDIN_FILENO, &buf) < 0) {
        err_sys("error fstat");
    }
    printf("文件所有者ID为：%d\n", buf.st_uid);
    printf("文件所属组ID为：%d\n", buf.st_gid);

    uid = getuid();
    euid = geteuid();
    gid = getgid();
    egid = getegid();

    if (uid == euid) {
        printf("进程有效用户ID等于进程实际用户ID：%d\n", uid);
    } else {
        printf("进程有效用户ID为：%d\n", uid);
        printf("进程实际用户ID为：%d\n", euid);
    }

    if (gid == egid) {
        printf("进程有效组ID等于进程实际组ID：%d\n", gid);
    } else {
        printf("进程有效组ID为：%d\n", gid);
        printf("进程实际组ID为：%d\n", egid);
    }

}

/**
 *[dendi875@localhost ctest]$ ls -l my_fstat
 *-rwxrwxr-x 1 dendi875 dendi875 7260 3月   7 18:33 my_fstat
 *
 *[dendi875@localhost ctest]$ id
 *uid=504(dendi875) gid=505(dendi875) 组=505(dendi875)
 *
 *[dendi875@localhost ctest]$ ./my_fstat < my_fstat
 *文件所有者ID为：504
 *文件所属组ID为：505
 *进程有效用户ID等于进程实际用户ID：504
 *进程有效组ID等于进程实际组ID：505
 *
 *给可执行程序加上SUID，并更改所有者为root
 *[dendi875@localhost ctest]$ su - root
 *[root@localhost ctest]# chown root my_fstat
 *[root@localhost ctest]# chmod u+s my_fstat
 *[root@localhost ctest]# ll my_fstat
 *-rwsrwxr-x 1 root dendi875 7456 3月   7 19:06 my_fstat
 *
 * 再恢复到普通用户运行
 *[root@localhost ctest]# exit
 *[dendi875@localhost ctest]$ ./my_fstat < my_fstat
 *文件所有者ID为：0
 *文件所属组ID为：505
 *进程有效用户ID为：504
 *进程实际用户ID为：0
 *进程有效组ID等于进程实际组ID：505
 */