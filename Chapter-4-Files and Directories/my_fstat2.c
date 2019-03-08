/**
 * 1.创建一个空洞文件
 * 2.使用fstat获取空洞文件的信息
 * 3.打印文件的st_size、st_blksize、st_blocks
 */

#include "apue.h"
#include <fcntl.h>

#define FILE_NAME "file.hole"

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;
    struct stat buf;

    if ((fd = creat(FILE_NAME, FILE_MODE)) < 0) {
        err_sys("creat error");
    }
    if (write(fd, buf1, 10) != 10) {
        err_sys("buf1 write error");
    }
    // 当前文件偏移量为10

    if (lseek(fd, 16384, SEEK_SET) == -1) {
        err_sys("lseek error");
    }
    // 现在文件偏移量为16384
    if (write(fd, buf2, 10) != 10) {
        err_sys("buf2 write error");
    }
    // 现在文件偏移量为16484
    if ((fstat(fd, &buf)) < 0) {
        err_sys("fstat error");
    }
    printf("%s 文件长度（以字节为单位）buf.st_size = %d\n", FILE_NAME, buf.st_size);
    printf("%s 文件buf.st_blksize = %d\n", FILE_NAME, buf.st_blksize);
    printf("%s 文件buf.st_blocks = %d\n", FILE_NAME, buf.st_blocks);

    exit(0);
}

/**
 * 使用exit(0)关闭进程。
 * 当一个进程终止时，内核会自动关闭它所有打开的文件。
 *
 * 实验：
[root@localhost ctest]# ./my_fstat2
file.hole 文件长度（以字节为单位）buf.st_size = 16394
file.hole 文件buf.st_blksize = 4096
file.hole 文件buf.st_blocks = 16

[root@localhost ctest]# ll file.hole
-rw-r--r-- 1 root root 16394 3月   7 21:55 file.hole
[root@localhost ctest]# wc -c file.hole
16394 file.hole

ll命令得到的16394是文件的长度，（wc命令验证）

st_blksize怎么理解？与dumpe2fs命令中的Block size有什么关系？
st_blocks怎么理解？
*************************************************
st_blocks是实际分配的的blocks字节数（每个已经定死了为512字节）这是POSIX定义的大小和gun dumpe2fs是
不一样的所以16*512 = 8192与下面du -s 命令计算的一样。

st_blksize对文件I/O性能最好的block大小。比如1个block大小为1K、2k、4k对文件I/O性能是不一样的，这里的
值是最优的那个。

注意：千万不要简单的认为buf.st_blksize * buf.st_blocks等于buf.st_size
**************************************************
[root@localhost ctest]# du -s file.hole
8       file.hole

说明改文件所使用的磁盘空间总量是8个block块（每个block为1024字节）共8192字节（与）。
也就是此文件有很多空洞
根据书本55页说明：
文件中的空洞并不要求在磁盘上占用存储区。当定位超出文件尾端之后写时，对于新写的数据需要分配
磁盘块，但是对于原文件尾端和新开始写位置之间的部分则不需要分配磁盘块。

du命令报告的是512字节还是1024字节取决于POSIXLY_CORRECT是否设置了（书本89页有说明）
df与df -P一样说明是设置了所以是8个block块（每个block为1024字节）

使用dumpe2fs 命令可以查看文件系统的信息。一个blocks是多大、一个inode是多大
[root@localhost ctest]# dumpe2fs /dev/sda2
Block size:               4096
*/