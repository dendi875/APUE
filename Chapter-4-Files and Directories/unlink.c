/**
 * 验证当有进程打开了一个文件时，其文件内容不会被删除
 */

#include "apue.h"
#include <fcntl.h>

int main(void)
{
    int fd;

    if ((fd = open("testfile", O_RDWR)) < 0) {
        err_sys("open error");
    }
    if (unlink("testfile") < 0) {
        err_sys("unlink error");
    }
    printf("file unlinked\n");

    sleep(30);
    printf("done\n");

    exit(0);
}

/**
 * 实验：
[root@localhost tmp]# ll testfile
-rw-r--r-- 1 root root 10240 3月   8 19:11 testfile
[root@localhost tmp]# df /tmp/
文件系统                 1K-块      已用      可用 已用% 挂载点
/dev/sda5             17414980   7846660   8683688  48% /
[root@localhost tmp]# df -i /tmp/
文件系统              Inode  已用(I)  可用(I) 已用(I)%% 挂载点
/dev/sda5            1105920  205769  900151   19% /
[root@localhost tmp]# ./unlink &
[1] 3794
[root@localhost tmp]# file unlinked

[root@localhost tmp]# ll testfile
ls: 无法访问testfile: 没有那个文件或目录
[root@localhost tmp]# df /tmp/
文件系统                 1K-块      已用      可用 已用% 挂载点
/dev/sda5             17414980   7846660   8683688  48% /
[root@localhost tmp]# df -i /tmp/
文件系统              Inode  已用(I)  可用(I) 已用(I)%% 挂载点
/dev/sda5            1105920  205769  900151   19% /
[root@localhost tmp]# done

[1]+  Done                    ./unlink
[root@localhost tmp]# df /tmp/
文件系统                 1K-块      已用      可用 已用% 挂载点
/dev/sda5             17414980   7846648   8683700  48% /
[root@localhost tmp]# df -i /tmp/
文件系统              Inode  已用(I)  可用(I) 已用(I)%% 挂载点
/dev/sda5            1105920  205768  900152   19% /

可以看到当有进程打开一个文件时，内容不会被删除。只有当进程关闭改文件或终止时（内核关闭该进程所打开的全部文件），该文件的内容才被删除
 */