/**
 *对于指定的文件描述符打印文件状态标志
 */
#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int val;
    if (argc != 2) {
        err_quit("usage：my_fcntl <descriptor#>");
    }

    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
        err_sys("fcntl error for fd：%d", atoi(argv[1]));
    }

    switch (val & O_ACCMODE) {
        case O_RDONLY:              // 00000000
            printf("read only");
            break;
        case O_WRONLY:              // 00000001
            printf("write only");
            break;
        case O_RDWR:                // 00000002
            printf("read write");
            break;
        default:
            err_dump("unknow access mode");
    }

    if (val & O_APPEND) {
        printf(", append");
    }
    if (val & O_NONBLOCK) {
        printf(", nonblocking");
    }
    if (val & O_SYNC) {
        printf(", synchronus writes");
    }

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if (val & O_FSYNC) {
        printf(", synchronus writes");
    }
#endif

    putchar('\n');

    exit(0);
}


/**
 * atoi()：把字符串转为数值。
 * 会跳过字符串开始处的空白字符，并且把后续字符看作是数（可能以加号或减号开头）的一部分，
 * 而且还会遇到第一个不属于数的字符处停止，如果不能执行转换（字符串为空，或者前导空白之后
 * 的字符形式不符合函数的要求），则返回0。
 *
 * O_ACCMODE：在 /usr/include/asm-generic/fcntl.h 为一个宏定义　00000003（八进制）
 *
 * 结果分析：
 *
 * $./my_fcntl 0
 *     read write
 * 获取文件描述符为0的文件状态标志
 * $./my_fcntl 1
 *     read write
 * 获取文件描述符为1的文件状态标志
 * $./my_fcntl 2
 *     read write
 * 获取文件描述符为2的文件状态标志
 * 任何一个进程都打开0（STDIN_FILENO），1（STDOUT_FILENO），2（STDERR_FILENO）这三个文件描述符，
 * 并且为可读写。
 *
 * $./my_fcntl 0 < /dev/tty
 *     read only
 * < /dev/tty 意思是重定向标准输入，也就是把标准输入重新指向另一个地方，原来是指向键盘现在是指向
 * /dev/tty。
   0 < /dev/tty 表示现在0这个文件描述符指向的是/dev/tty的文件表项，所关联的是/dev/tty的i节点

 * $./my_fcntl 1 > temp.foo
 *     write only
 * > temp.foo 意思是重定向标准输出，也就是把标准输出重新指向另一个地方，原来是指向屏幕现在被重定向
 * 指到了 temp.foo。
 * 1 > temp.foo 表示现在1这个文件描述符指向的是temp.foo这个文件表项，所关联的是temp.foo的i节点
 *
 *$./my_fcntl 2 2>>temp.foo
 *     write only, append
 * 2>>temp.fo 意思是以追加方式重定向标准错误，也就是以追加的方式把标准错误重新指向另一个地方
 * 原来是屏幕现在是temp.foo文件。
 * 2 2>>temp.foo 表示现在2这个文件描述符向的是temp.foo这个文件表项，所关联的temp.foo的不节点
 *
 * $./my_fcntl 3
 *     fcntl error for fd：3: Bad file descriptor
 * fd = 3，没有被使用
 *
 * $./my_fcntl 5 5<>temp.foo
 *     read write
 *
 * >temp.foo从$./my_fcntl 1 > temp.foo可以知道如果打开temp.foo那么有write only状态标志。
 * 5 < 从$./my_fcntl 0 < /dev/tty可以知道如果打开temp.foo那么有read only状态标志并且打开temp.foo的
 * 文件描述符定死了为5（默认是系统分配一个未使的最小数值）。结合起来 5<>temp.foo 就是以读写方式打开文件
 * temp.foo并且指定了文件描述符为5。
 * <与>之间不能有空格
 */