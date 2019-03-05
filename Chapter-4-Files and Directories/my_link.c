/**
 * link()函数创建一个文件的硬链接
 */
#include "apue.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        err_quit("usage：my_link <oldpath> <newpath>");
    }
    if (link(argv[1], argv[2]) < 0) {
        err_sys("link error for oldpath：%s newpath：%s", argv[1], argv[2]);
    }
    exit(0);
}

/**
 * 做如下实验：
 * 一、编译该文件
 * [root@localhost Chapter-4-Files and Directories]# gcc -I../ my_link.c -o my_link
 *
 * 二、把编译好的文件拷贝到 /tmp目录下
 *[root@localhost Chapter-4-Files and Directories]# cp -a my_link /tmp/
 *
 * 三、把/etc/passwd文件拷贝到 /tmp目录下做实验
 * [root@localhost Chapter-4-Files and Directories]# cd /tmp/
 * [root@localhost tmp]# cp -a /etc/passwd ./
 *
 * 四、查看 /tmp目录下的总的容量和已使用的inode数
 * [root@localhost tmp]# du -sb; df -i .
 * 73774   .
 * 文件系统              Inode  已用(I)  可用(I) 已用(I)%% 挂载点
 * /dev/sda5            1105920  205756  900164   19% /
 *
 * 五、运行my_link
 * [root@localhost tmp]# ./my_link passwd passwd-hd
 *
 * 六、再查看/tmp目录下的总的容量和已使用的inode数
 * [root@localhost tmp]# du -sb; df -i .
 * 73774   .
 * 文件系统              Inode  已用(I)  可用(I) 已用(I)%% 挂载点
 * /dev/sda5            1105920  205756  900164   19% /
 *
 * [root@localhost tmp]# ls -li passwd*
 * 690666 -rw-r--r-- 2 root root 2015 2月  26 20:27 passwd
 * 690666 -rw-r--r-- 2 root root 2015 2月  26 20:27 passwd-hd
 *
 * 与第四步结果对比可以得到，/tmp目录下容量没增加，已用的inode数量也没增加。
 * 因为硬链接（passwd-hd）不是创建新的文件，只是在某目录（/tmp）的block中记录该文件的文件名，并且与某
 * inode关联起来（inode使用的是/tmp/passwd的inode）。
 *
 * 1. 不是创建新的文件操作系统自然不会为硬链接分配新的inode号，那df -i .看到的已用inode数量也就不会改变
 * 2. 操作系统会为每个目录分配一个inode和至少一个block，目录的inode里包含目录的权限和属性相关信息，包含
 * 改目录的block号码；目录的block里存放该目录下文件名称和文件名称对应的inode号码。因为硬链接只是在目录的
 * block里记录文件名和文件名对应的inode号，所以目录容量几乎不变。
 *
 * 备注：使用绝对路径方式运行也行
 * [root@localhost Chapter-4-Files and Directories]# ./my_link /tmp/passwd /tmp/passwd-hd
 * [root@localhost Chapter-4-Files and Directories]# ls -li /tmp/passwd*
 * 696856 -rw-r--r-- 2 root root 2015 2月  26 20:27 /tmp/passwd
 * 696856 -rw-r--r-- 2 root root 2015 2月  26 20:27 /tmp/passwd-hd
 */