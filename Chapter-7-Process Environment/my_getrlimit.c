#include "apue.h"
#include <sys/resource.h>

#define doit(name) pr_limit(#name, name)

static void pr_limit(char *, int);

int main(void)
{
    doit(RLIMIT_NOFILE);
    doit(RLIMIT_STACK);

    exit(0);
}

static void pr_limit(char *name, int resource)
{
    struct rlimit limit;
    unsigned long long lim;

    if (getrlimit(resource, &limit) < 0) {
        err_sys("getrlimit error for %s", name);
    }
    printf("%-14s    ", name);
    if (limit.rlim_cur == RLIM_INFINITY) {
        printf("(infinite)  ");
    } else {
        lim = limit.rlim_cur;
        printf("%10lld  ", lim);
    }
    if (limit.rlim_max == RLIM_INFINITY) {
        printf("(infinite)");
    } else {
        lim = limit.rlim_max;
        printf("%10lld", lim);
    }
    putchar((int)'\n');
}

/*实验：
[dendi875@localhost Chapter-7-Process Environment]$ ./my_getrlimit
RLIMIT_NOFILE           1024        4096
RLIMIT_STACK        10485760  (infinite)

[dendi875@localhost Chapter-7-Process Environment]$ cat /proc/self/limits
Limit                     Soft Limit           Hard Limit           Units
Max cpu time              unlimited            unlimited            seconds
Max file size             unlimited            unlimited            bytes
Max data size             unlimited            unlimited            bytes
Max stack size            10485760             unlimited            bytes
Max core file size        0                    unlimited            bytes
Max resident set          unlimited            unlimited            bytes
Max processes             1024                 7932                 processes
Max open files            1024                 4096                 files
Max locked memory         65536                65536                bytes
Max address space         unlimited            unlimited            bytes
Max file locks            unlimited            unlimited            locks
Max pending signals       7932                 7932                 signals
Max msgqueue size         819200               819200               bytes
Max nice priority         0                    0
Max realtime priority     0                    0
Max realtime timeout      unlimited            unlimited            us*/