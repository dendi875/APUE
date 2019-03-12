/**
 * pathconf函数实例；测试当前系统文件名或目录名的最大长度（最大字节数）一个字符一个字节，
 * 不包括'\0'字节
 */

#include "apue.h"
#include <errno.h>

#define TEST_PATH  "/"

int main(void)
{
    long val;

    errno = 0;
    if ((val = pathconf(TEST_PATH, _PC_NAME_MAX)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL) {
                fputs("no supported\n", stdout);
            } else {
                err_sys("pathconf error, path = %s", TEST_PATH);
            }
        } else {
            fputs("no limit\n", stdout);
        }
    } else {
        printf("NAME_MAX = %ld\n", val);
    }
}
/*
实验：
[root@localhost Chapter-2-Standardization and Implementations]# ./my_pathconf
NAME_MAX = 255*/