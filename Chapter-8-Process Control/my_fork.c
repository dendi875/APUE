#include "apue.h"

int main(void)
{
    pid_t pid;
    char buf1[] = "parent first write to stdout\n";
    char buf2[] = "parent second write to stdout\n";

    if (write(STDOUT_FILENO, buf1, sizeof(buf1) - 1) != sizeof(buf1) - 1) {
        err_sys("write buf1 error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {  /* 子进程执行逻辑 */
        char buf3[] = "child write to stdout\n";
        if (write(STDOUT_FILENO, buf3, sizeof(buf3) - 1) != sizeof(buf3) - 1) {
            err_sys("write buf3 error");
        }
        exit(0);
    } else {                /* 父进程执行逻辑 */
        sleep(2);
    }

    // 当标准输出被重定向时，它是全缓冲的，当执行exit函数时执行标准I/O清理，在冲洗缓冲区
    printf("%s", buf2);

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-8-Process Control]$ ./my_fork > test.out
[dendi875@localhost Chapter-8-Process Control]$ cat test.out
parent first write to stdout
child write to stdout
parent second write to stdout

这个实验可以证明子进程从父进程那里复制过来的文件描述符是和父进程的文件描述符指向同一个文件
表项（也就共享同一文件偏移量），写的时候不会相互覆盖*/