/**
 * uname函数实例
 */

#include "apue.h"
#include <sys/utsname.h>

int main(void)
{
    struct utsname buf;

    if (uname(&buf) < 0) {
        err_sys("uname error");
    }
    printf("Operating system name：%s\n", buf.sysname);
    printf("network node hostname：%s\n", buf.nodename);
    printf("kernel release：%s\n", buf.release);
    printf("kernel version：%s\n", buf.version);
    printf("machine hardware name：%s\n", buf.machine);

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-6-System Data Files and Information]$ ./my_uname
Operating system name：Linux
network node hostname：localhost.localdomain
kernel release：2.6.32-279.el6.i686
kernel version：#1 SMP Fri Jun 22 10:59:55 UTC 2012
machine hardware name：i686

[dendi875@localhost Chapter-6-System Data Files and Information]$ uname -a
Linux localhost.localdomain 2.6.32-279.el6.i686 #1 SMP Fri Jun 22 10:59:55 UTC 2012 i686 i686 i386 GNU/Linux*/