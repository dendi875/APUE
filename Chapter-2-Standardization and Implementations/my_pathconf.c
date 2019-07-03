/**
 * pathconf函数实例
 */

#include "apue.h"
#include <errno.h>

#define TEST_PATH  "/"

static void pr_sysconf(char *,  int);
static void pr_pathconf(char *, char *, int);

int main(void)
{

    // 测试当前系统文件名或目录名的最大长度（最大字节数）一个字符一个字节，不包括'\0'字节
    pr_pathconf("当前系统文件名或目录名的最大长度（NAME_MAX） =", TEST_PATH, _PC_NAME_MAX);

    // 测试系统默认的nice值
#if defined(NZERO)
    printf("系统默认的nice值（NZERO） =%d\n", NZERO);
#elif defined(_SC_NZERO)
    pr_sysconf("系统默认的nice值（NZERO） =", _SC_NZERO);
#else
    #error NZERO undefined
#endif

    // 查看系统每秒钟的时钟滴答数（clock ticks）
    pr_sysconf("每秒钟的滴答数（CLK_TCK）=", _SC_CLK_TCK);

    // 查看一个进程可以创建最大的线程数
    pr_sysconf("进程可以创建的最大线程数（THREADS_MAX）=", _SC_THREAD_THREADS_MAX);

    // 查看一个线程的栈可用的最小字节数
    pr_sysconf("一个线程的栈可用的最小字节数（STACK_MIN）=", _SC_THREAD_STACK_MIN);

    // 查看当前系统虚拟内存页大小，以字节为单位
    pr_sysconf("虚拟内存页大小，字节为单位（SC_PAGE_SIZE）=", _SC_PAGE_SIZE);

    // 查看 readv或writev函数可以使用最多的 iovec 结构体的个数
    pr_sysconf("readv或writev函数可以使用最多的 iovec 结构体的个数（IOV_MAX） =", _SC_IOV_MAX);

    // 查看每个进程能够打开的最大文件数
    pr_sysconf("进程最大打开文件数（SC_OPEN_MAX）=", _SC_OPEN_MAX);

}

static void pr_sysconf(char *mesg,  int name)
{
    long val;

    fputs(mesg, stdout);
    errno = 0;
    if ((val = sysconf(name)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL) {
                fputs(" (no supported)\n", stdout);
            } else {
                err_sys("sysconf error");
            }
        } else {    // 返回-1但errno值没改变
            fputs(" (no limit)\n", stdout);
        }
    } else {
        printf(" %ld\n", val);
    }
}

static void pr_pathconf(char *mesg, char *path, int name)
{
    long val;

    fputs(mesg, stdout);
    errno = 0;
    if ((val = pathconf(path, name)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL) {
                fputs(" (no supported)\n", stdout);
            } else {
                err_sys("pathconf error, path = %s", path);
            }
        } else {                            /* 返回-1但errno值没改变 */
            fputs(" (no limit)\n", stdout);
        }
    } else {
        printf(" %ld\n", val);
    }
}

/*
实验：
[dendi875@localhost Chapter-2-Standardization and Implementations]$ ./my_pathconf
当前系统文件名或目录名的最大长度（NAME_MAX） = 255
系统默认的nice值（NZERO） = 20
每秒钟的滴答数（CLK_TCK）= 100
进程可以创建的最大线程数（THREADS_MAX）= (no limit)
一个线程的栈可用的最小字节数（STACK_MIN）= 16384
虚拟内存页大小，字节为单位（SC_PAGE_SIZE）= 4096
readv或writev函数可以使用最多的 iovec 结构体的个数（IOV_MAX） = 1024
进程最大打开文件数（SC_OPEN_MAX）= 1024

*/