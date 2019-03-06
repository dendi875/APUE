/**
 * 测试一个进程最多可以打开多少个文件描述符
 */
#include "apue.h"
#include <fcntl.h>

#define NAME_LEN 1024
#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) // 666

int main(void)
{
    int fd;
    int i = 0;
    char filename[NAME_LEN];

    umask(0);
    while (1) {
        sprintf(filename, "testfile%d", ++i);
        fd = open(filename, O_CREAT, RWRWRW);
        if (fd < 0) {
            err_sys("open error");
        }
        printf("%d\n", i);
    }

    exit(0);
}



/**
 * sprintf()类似于printf，不同之处就是sprintf把输出写入到第一个参数指向的字符数组中而不是流中。
 * 当完成写入时，sprintf会添加一个空字符，并且返回存储到filename数组中的字符数量（不计空字符）
 * 如果遇到错误，sprintf返回负值。
 * sprintf()还可以用来把数转成字符格式，整型的6转为'6'。
 *
 * 运行结果：
 * [root@localhost Chapter-3-File IO]# ./my_maxopen
 * ...
 * 1021
 * open error: Too many open files
 *
 * 查看一个进程最多可以打开多少个文件
 * [root@localhost Chapter-3-File IO]# ulimit -a
 *
 * 1024-1021 = 3　每个进程默认打开三个描述符0（STDIN_FILENO）1（STDOUT_FILENO）2（STDERR_FILENO）
 *
 * 可以使用ulimit -n x修改默认的值
 * [root@localhost Chapter-3-File IO]# ulimit -n 4096
 * [root@localhost Chapter-3-File IO]# ./my_maxopen
 * 4093
 * open error: Too many open files
 *
 * 是不是可以用ulimit修改成无限大的值？（理论上是可以的只要系统配置足够好，内存足够大，文件描述符
 * 是占内存的），可以查看当前系统所运行的最大文件数
 * [root@localhost Chapter-3-File IO]# cat /proc/sys/fs/file-max
 * 100837
 */