/**
 * gethostname函数实例
 */

#include "apue.h"

#define HOST_NAME_SIZE 64

int main(void)
{
    char name[HOST_NAME_SIZE];

    if (gethostname(name, HOST_NAME_SIZE) < 0) {
        err_sys("gethostname error");
    }
    printf("name = %s\n", name);

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-6-System Data Files and Information]$ ./my_gethostname
name = localhost.localdomain

[dendi875@localhost Chapter-6-System Data Files and Information]$ hostname
localhost.localdomain*/
