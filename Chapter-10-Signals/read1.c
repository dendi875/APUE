#include "apue.h"
#include <unistd.h>

static void sig_alrm(int);

int main(void)
{
    int    n;
    char   line[MAXLINE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        err_sys("signal(SIGALRM) error");
    }

    alarm(10);
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0) {
        err_sys("read error");
    }
    alarm(10);

    write(STDOUT_FILENO, line, n);

    exit(0);
}

static void sig_alrm(int signo)
{
    /* nothing to do, just return to interrupt the read */
}

/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./read1
read error: Interrupted system call


底速系统调用是可能会使进程永远阻塞的一类系统调用，包括：
1、如果某些类型文件（如读管道、终端设备和网络设备）的数据不存在，则读操作可能会使调用者永远阻塞
2、如果这些数据不能被相同的类型文件立即接受，则写操作可能会使调用都永远阻塞
3、在某种条件发生之前打开某些类型文件，可能会发生阻塞（例如打开一个终端设备，需要先等待与之连接的
调制解调器应答）
4、pause函数和wait函数
5、某些ioctl操作
6、某些进程间通信函数

这个例子就是终端设备（标准输入）没数据，然后read就被阻塞。

中断的系统调用：
如果进程在执行一个底速系统调用而阻塞期间捕捉到一个信号，则该系统调用就被中断不再继续执行。
该系统调用返回出错，其errno设置为EINTR。
*/