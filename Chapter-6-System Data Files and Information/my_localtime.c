/**
 * localtime函数使用实例
 */

#include "apue.h"
#include <time.h>

int main(void)
{
    time_t current = time(NULL);
    struct tm *ptr;

    if ((ptr = localtime(&current)) == NULL) {
        err_sys("localtime error");
    }

    printf("%.2d-%.2d-%.2d %2d:%2d:%2d\n", ptr->tm_mon + 1, ptr->tm_mday, ptr->tm_year + 1900,
        ptr->tm_hour, ptr->tm_min, ptr->tm_sec
    );

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-6-System Data Files and Information]$ ./my_localtime
03-15-2019 20:43:23*/