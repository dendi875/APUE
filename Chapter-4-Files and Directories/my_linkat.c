/**
 * linkat() 相对于文件描述符（目录的描述符）来创建硬链接
 *
 * 书本上没有这个函数的实例；自己写一个方便记忆和理解
 */

#include "apue.h"
#include <fcntl.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *olddp, *newdp;
    int olddirfd, newdirfd;

    if (argc != 5) {
        err_quit("usage：my_linkat <oldpath> <old_file_name> <newpath> <new_file_name>");
    }
    if ((olddp = opendir(argv[1])) == NULL) {
        err_sys("can't open %s", argv[1]);
    }
    if ((newdp = opendir(argv[3])) == NULL) {
        err_sys("can't open %s", argv[3]);
    }
    if ((olddirfd = dirfd(olddp)) < 0 || (newdirfd = dirfd(newdp)) < 0) {
        err_sys("dirfd error");
    }

    if (linkat(olddirfd, argv[2], newdirfd, argv[4], 0) < 0) {
       err_sys("link error for old_file_name：%s new_file_name：%s", argv[2], argv[4]);
    }

    closedir(olddp);
    closedir(newdp);

    exit(0);
}


/**
 * 做如下实验：
 * 一、编译该文件
 * [root@localhost Chapter-4-Files and Directories]# gcc -I../ my_linkat.c -o my_linkat
 *
 * 二、把编译好的文件拷贝到 /tmp目录下
 *[root@localhost Chapter-4-Files and Directories]# cp -a my_linkat /tmp/
 *
 * 三、在/tmp目录下新建两个测试目录，把/etc/passwd文件拷贝到某个测试目录中
 * [root@localhost Chapter-4-Files and Directories]# cd /tmp/
 * [root@localhost tmp]# mkdir {testdir1,testdir2}
 * [root@localhost tmp]# cp -a /etc/passwd testdir1/
 *
 * 四、查看 /tmp目录下的总的容量和已使用的inode数
 *  84943   .
 *  文件系统              Inode  已用(I)  可用(I) 已用(I)%% 挂载点
 *  /dev/sda5            1105920  205763  900157   19% /

 * 五、运行my_linkat
 * [root@localhost tmp]# ./my_linkat testdir1 passwd testdir2 passwd-h
 *
 * 六、再查看/tmp目录下的总的容量和已使用的inode数
 * [root@localhost tmp]# du -sb; df -i .
 * 84943   .
 * 文件系统              Inode  已用(I)  可用(I) 已用(I)%% 挂载点
 * /dev/sda5            1105920  205763  900157   19% /
 *
 * [root@localhost tmp]# ls -li testdir1/passwd  testdir2/passwd-hd
 * 662025 -rw-r--r-- 2 root root 2015 2月  26 20:27 testdir1/passwd
 * 662025 -rw-r--r-- 2 root root 2015 2月  26 20:27 testdir2/passwd-hd
 *
 * 与第四步结果对比可以得到，/tmp目录下容量没增加，已用的inode数量也没增加。
 * 因为硬链接（passwd-hd）不是创建新的文件，只是在某目录（/tmp/testdir2）的block中记录该文件的文件名，
 * 并且与某inode关联起来（inode使用的是/tmp/testdir1/passwd的inode）。
 *
 * 1. 不是创建新的文件操作系统自然不会为硬链接分配新的inode号，那df -i .看到的已用inode数量也就不会改变
 * 2. 操作系统会为每个目录分配一个inode和至少一个block，目录的inode里包含目录的权限和属性相关信息，包含
 * 改目录的block号码；目录的block里存放该目录下文件名称和文件名称对应的inode号码。因为硬链接只是在目录的
 * block里记录文件名和文件名对应的inode号，所以目录容量几乎不变。
 */