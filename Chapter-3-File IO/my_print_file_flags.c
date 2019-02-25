/**
 *打印一个文件描述符的文件状态标志
 */
#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int val;

    if (argc != 2) {
        err_quit("usage：my_print_file_flags <descriptor#>");
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
 * O_ACCMODE：在 /usr/include/asm-generic/fcntl.h 为一个宏定义　00000003
 *
 * 结果分析：
 *
 * $./my_print_file_flags 0
 *     read write
 * 获取文件描述符为0的文件状态标志
 * $./my_print_file_flags 1
 *     read write
 * 获取文件描述符为1的文件状态标志
 * $./my_print_file_flags 2
 *     read write
 * 获取文件描述符为2的文件状态标志
 * 默认的0（STDIN_FILENO），1（STDOUT_FILENO），2（STDERR_FILENO）都在正在使用的，并且为可读写。
 * $./my_print_file_flags 0 < /dev/tty
 *     read only
 * my_print_file_flags程序不会认为正在从/dev/tty中读取数据，它会认为从0（STDIN_FILENO）标准输入流中获取的数据是从键盘上录入的。
 * 对标准输入进行重定向，标准输入来自于/dev/tty，将默认的读写改变为只读
 * $./my_print_file_flags 1 > temp.foo
 *     write only
 * 对标准输出进行重定向，标准输出到文件temp.foo中，将默认的读写改变为只写
 *$./my_print_file_flags 2 2>>temp.foo
 *     write only, append
 * $./my_print_file_flags 3
 *     fcntl error for fd：3: Bad file descriptor
 * fd = 3，没有被使用
 * $./my_print_file_flags 5 5<>temp.foo
 *     read write
 */