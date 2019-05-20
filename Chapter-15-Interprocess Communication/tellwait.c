/**
 * 使用管道实现父子进程之间同步
 * 思路：
 * fork 之前创建两条管道（TELL_WAIT）
 * 父进程向第一条管道的写端写（TELL_CHILD），子进程向第一条管道的读端读（WAIT_PARENT）
 * 子进程向第二条管道的写端写（TELL_PARENT），父进程向第二条管道的读端读（WAIT_CHILD）
 */

#include "apue.h"

static int pfd1[2], pfd2[2];

void TELL_WAIT(void);
void TELL_CHILD(pid_t pid);
void TELL_PARENT(pid_t pid);
void WAIT_CHILD(void);
void WAIT_PARENT(void);
static void charatatime(char *);

int main(void)
{
    pid_t   pid;

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {  /* 子进程中 */
        WAIT_PARENT();
        charatatime("output from child\n");
        TELL_PARENT(getppid());
    } else {    /* 父进程中 */
        charatatime("output from parent\n");
        TELL_CHILD(pid);
        WAIT_CHILD();
    }

    exit(0);
}

void TELL_WAIT(void)
{
    if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
        err_sys("pipe error");
    }
}

void TELL_CHILD(pid_t pid)
{
    if (write(pfd1[1], "p", 1) != 1) {
        err_sys("write error");
    }
}

void TELL_PARENT(pid_t pid)
{
    if (write(pfd2[1], "c", 1) != 1) {
        err_sys("write error");
    }
}

void WAIT_CHILD(void)
{
    char    c;

    if (read(pfd2[0], &c, 1) < 0) {
        err_sys("read error");
    }

    if (c != 'c') {
        err_quit("WAIT_CHILD: incorrect data");
    }
}

void WAIT_PARENT(void)
{
    char    c;

    if (read(pfd1[0], &c, 1) < 0) {
        err_sys("read error");
    }

    if (c != 'p') {
        err_quit("WAIT_PARENT：incorrect data");
    }
}

static void charatatime(char *str)
{
    char *ptr;
    int c;

    /**
     * 关闭标准输出的默认行缓冲，这样每次调用fpuc都会调用底层系统调用函数write
     */
    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr) != '\0'; ptr++) {
        fputc(c, stdout);
    }
}

/*
实验：
[dendi875@localhost Chapter-15-Interprocess Communication]$ ./tellwait; ./tellwait; ./tellwait;
output from parent
output from child
output from parent
output from child
output from parent
output from child
*/