/**
 * 从标准输入读命令并执行
 */
#include "apue.h"
#include <sys/wait.h>

/**
 * 信号处理函数
 */
static void sig_int(int);

int main(int argc, char **argv)
{
    pid_t pid;
    int status;
    char buf[MAXLINE];

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        err_sys("signal error");
    }

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = 0;
        }

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) {
            execlp(buf, buf, (char *)NULL);
            err_ret("couldn't execute：%s", buf);
            exit(127);
        }

        // 父进程执行逻辑
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            err_sys("waitpid error");
        }
        printf("%% ");
    }

    exit(0);
}

void sig_int(int signo)
{
    printf("interrupt\n%%"); // interrupt 中断
}


/**
 * signal()函数 第一个参数是int类型，第二个参数是函数指针，这个指针指向一个函数，它存储函数的
 * 地址，存储的函数需要满足有一个形参（int），返回值为void，
 * 返回值：出错时返回SIG_ERR。
 *
 *实验：
 * [dendi875@localhost Chapter-1-System Overview]$ ./signal
 * % date
 * 2019年 03月 07日 星期四 10:14:20 CST
 * % pwd
 * /mnt/hgfs/www/c_exercise/APUE/Chapter-1-System Overview
 * % ^Cinterrupt
 */