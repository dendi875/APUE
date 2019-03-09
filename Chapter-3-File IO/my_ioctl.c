/**
 * ioctl函数使用实例，获取终端窗口的大小。书本第18.12节有讲解
 */

#include "apue.h"
#include <termios.h>
#include <sys/ioctl.h>

int main(void)
{
    struct winsize size;

    if (isatty(STDOUT_FILENO) == 0) {
        exit(1);
    }
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
        err_sys("TIOCGWINSZ error");
    }
    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);

    exit(0);
}

/**
不同的设备提供就有不同的驱动程序来提供这个设备专用的一组ioctl命令，应用层使用ioctl调用内核
代码，内核代码再根据不同的设备来调用不同的驱动程序代码，最终有设备的驱动程序代码来获取
和设置设备文件的物理特性。

ioctl函数为不同种类的设备提供通用的ioctl命令。

实验：
[root@localhost Chapter-3-File IO]# ./my_ioctl
43 rows, 189 columns
[root@localhost Chapter-3-File IO]# ./my_ioctl
20 rows, 132 columns

串口文件叫做/dev/ttyS0
显示器文件叫做 /dev/fd0
 */