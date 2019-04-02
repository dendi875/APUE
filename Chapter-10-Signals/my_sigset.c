/**
 * 信号集
 */

#include <stdio.h>
#include <signal.h>

int main(void)
{
    printf("%lu\n", (unsigned long)sizeof(sigset_t));

    return 0;
}

/*[dendi875@localhost Chapter-10-Signals]$ ./my_sigset
128

[dendi875@localhost ~]$ find /usr/include/ -type f | xargs grep --color=auto -n sigset_t
信号集是一种数据类型：
typedef unsigned long sigset_t;*/