/**
 * ttyname查看不同的终端所对应的设备文件名
 */
#include "apue.h"

int main(void)
{
    printf("fd 0：%s\n", ttyname(0));
    printf("fd 1：%s\n", ttyname(1));
    printf("fd 2：%s\n", ttyname(2));

    return 0;
}

/*每个进程都可以通过一个特殊的设备文件/dev/tty访问它的控制终端。事实上每个终端设备都对应一个不同
的设备文件，/dev/tty提供了一个通用的接口，一个进程要访问它的控制终端可以通过/dev/tty也可以通过
该终端对应的设备文件来访问。ttyname函数可以由文件描述符查出对应的文件名，该文件描述符必须指向
一个终端设备而不能是任意文件。

实验：
[dendi875@localhost Chapter-9-Process Relationships]$ ./my_ttyname
fd 0：/dev/pts/0
fd 1：/dev/pts/0
fd 2：/dev/pts/0
*/