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
NAME_MAX = 255


*/