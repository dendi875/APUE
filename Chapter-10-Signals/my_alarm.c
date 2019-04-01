#include "apue.h"

int main(void)
{
    int counter = 0;

    alarm(1);
    while(1) {
        printf("%ld\n", counter);
        counter++;
    }

    return 0;
}

/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_alarm
0
1
2
...
30130闹钟
-------------------
alarm设置一个定时器（闹钟时间），当定时器超时时，内核将产生SIGALRM信号。如果程序不忽略或不捕捉此信号，则系统默认的处理动作是终止调用alarm函数的进程。

man 7 signal可以看到SIGALRM信号系统默认处理动作是终止进程
*/