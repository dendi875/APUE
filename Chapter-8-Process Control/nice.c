#include "apue.h"
#include <errno.h>
#include <sys/time.h>

#if defined(MACOS)
#include <sys/syslimits.h>
#elif defined(SOLARIS)
#include <limits.h>
#elif defined(BSD)
#include <sys/param.h>
#endif

unsigned long long count;
struct timeval end;

void checktime(char *);

int main(int argc, char *argv[])
{
    pid_t   pid;
    char    *s;
    int     nzero, ret;
    int     adj = 0;

    setbuf(stdout, NULL);   /* 关闭标准输出默认的行缓冲 */

#if defined(NZERO)
    nzero = NZERO;
#elif defined(_SC_NZERO)
    nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif
    printf("NZERO = %d\n", nzero);

    if (argc == 2) {
        adj = strtol(argv[1], NULL, 10);
    }
    gettimeofday(&end, NULL);
    end.tv_sec += 10;       /* 父子进程分别运行10秒钟 */

    if ((pid = fork()) < 0) {
        err_sys("fork failed");
    } else if (pid == 0) {  /* 子进程中运行的代码 */
        s = "child";
        printf("current nice value in child is %d, adjusting by %d\n", nice(0) + nzero, adj);
        errno = 0;
        if ((ret = nice(adj)) == -1 && errno != 0) {
            err_sys("child set scheduling priority");
        }
        printf("now child nice value is %d\n", ret+nzero);
    } else {
        s = "parent";
        printf("current nice value int parent is %d\n", nice(0)+nzero);
    }

    for (;;) {
        if (++count == 0) {
            err_quit("%s counter wrap", s);
        }
        checktime(s);
    }
}

void checktime(char *str)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    if (tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec) {
        printf("%s count = %lld\n", str, count);
        exit(1);
    }
}

/*实验：
[dendi875@localhost Chapter-8-Process Control]$ ./nice
NZERO = 20
current nice value int parent is 20
current nice value in child is 20, adjusting by 0
now child nice value is 20
child count = 27339917
parent count = 27529838

增加子进程的nice值（子进程优先级被降低，被CPU所执行的次数就少，count值就小）
可以看到父进程占用了94.2%的CPU资源，而子进程占用了1.2%
[root@localhost C]# ps aux | grep nice
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
dendi875 13804 94.2  0.0   1864   412 pts/0    R+   19:37   0:08 ./nice 20
dendi875 13805  1.2  0.0   1864   240 pts/0    RN+  19:37   0:00 ./nice 20
root     13830  0.0  0.0   5976   752 pts/1    S+   19:37   0:00 grep --color=auto nice*/