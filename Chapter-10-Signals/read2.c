#include "apue.h"
#include <setjmp.h>
#include <unistd.h>

static void     sig_alrm(int);
static jmp_buf  env_alrm;

int main(void)
{
    int    n;
    char   line[MAXLINE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        err_sys("signal(SIGALRM) error");
    }
    if (setjmp(env_alrm) != 0) {
        err_quit("read timeout");
    }

    alarm(10);
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0) {
        err_sys("read error");
    }
    alarm(0);

    write(STDOUT_FILENO, line, n);

    exit(0);
}

static void  sig_alrm(int signo)
{
    longjmp(env_alrm, 1);
}

/*[dendi875@localhost Chapter-10-Signals]$ ./read2
read timeout

这个例子不会像read1.c那样会出现中断想系统调用，当alarm超时时，发送SIGALRM信号然后信号处理程序sig_alrm
直接longjmp跳转到setjmp处，setjmp返回值为1。*/