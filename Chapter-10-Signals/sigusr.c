/**
 * 捕捉SIGUSR1和SIGUSR2这两个用户自定义信号
 */

#include "apue.h"

static void sig_usr(int);   /* SIG_USR1和SIG_USR2信号处理函数 */

int main(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        err_sys("can't catch SIGUSR1");
    }
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        err_sys("can't catch SIGUSR2");
    }
    for (;;) {
        pause();
    }

    return 0;
}

static void sig_usr(int signo)
{
    if (signo == SIGUSR1) {
        printf("received SIGUSR1\n");
    } else if (signo == SIGUSR2) {
        printf("received SIGUSR2\n");
    } else {
        err_dump("received signal %d\n", signo);
    }
}

/*SIGUSR1、SIGUSR2用户自定义信号。
pause使调用进程挂起直到捕捉到一个信号。只有执行了一个信号处理程序并从其返回时，pause才返回。

把进程放到后台进程组中运行
[dendi875@localhost Chapter-10-Signals]$ ./sigusr &
[1] 26200

查看后台作业，可以看到进程在后台运行
[dendi875@localhost Chapter-10-Signals]$ jobs -l
[1]+ 26200 Running                 ./sigusr &

ps命令也能验证sigusr程序进程在后台
[dendi875@localhost Chapter-10-Signals]$ ps ajxf | grep sigusr
17600 26200 26200 17600 pts/0    26203 S      504   0:00              \_ ./sigusr
17600 26204 26203 17600 pts/0    26203 S+     504   0:00              \_ grep --color=auto sigusr

给进程发送SIGUSR1信号，信号发送后，因为我们注册了SIGUSR1信号处理程序，所以当信号处理程序执行后，pause
返回。继续下一次for
[dendi875@localhost Chapter-10-Signals]$ kill -SIGUSR1 26200
[dendi875@localhost Chapter-10-Signals]$ received SIGUSR1

给进程发送SIGUSR2信号
[dendi875@localhost Chapter-10-Signals]$ kill -SIGUSR2 26200
[dendi875@localhost Chapter-10-Signals]$ received SIGUSR1

[dendi875@localhost Chapter-10-Signals]$ kill -SIGUSR2 26200
[dendi875@localhost Chapter-10-Signals]$ received SIGUSR2

终止进程
[dendi875@localhost Chapter-10-Signals]$ kill 26200

注意：SIGUSR1、SIGUSR2信号系统默认处理动作时终止程序，因为我们这里注册了这两个信号的处理程序
（捕捉该信号），所以不会终止。

kill默认发送的是 SIGTERM（15）信号，我们没有捕捉该信号，那么系统就使用该信号的默认处理动作（优雅的终止进程）*/