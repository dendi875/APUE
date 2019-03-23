#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc != 2) {
        err_quit("usage：my_wrapper <filename>");
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        err_sys("open error");
    }

    /**
     * 先关闭STDIN_FILENO，然后使STDIN_FILENO指向与fd相同的文件表项
     */
    if (dup2(fd, STDIN_FILENO) < 0) {
        err_sys("dup2 error");
    }
    close(fd);      /* 现在就达到了使标准输入重定向到文件fd中 */

    if (execl("./my_upper", "my_upper", (char *)0) < 0) {
        err_sys("execl error");
    }

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-8-Process Control]$ echo 'hello' > testfile.in
[dendi875@localhost Chapter-8-Process Control]$ ./my_wrapper testfile.in
HELLO

这个实验就可以解释书本第66页、201页关于文件描述符标志的意思。
文件描述符标志是控制在执行exec时该文件是关闭该文件描述符还是不关闭。

进程中每个打开的文件描述符都有一个执行中关闭标志。若设置了些标志（值为1），则在执行
exec时关闭该描述符；否则该描述符仍打开。除非特地用fcntl设置了该执行时关闭标志，否则
系统默认操作是在exec后仍保持这种描述符打开。
*/