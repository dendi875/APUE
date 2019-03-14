#include "apue.h"
#include <errno.h>

void make_temp(char *);

int main(void)
{
    char good_template[] = "/tmp/dirXXXXXX";
    char *bad_template = "/tmp/dirXXXXXX";

    printf("trying to create first temp file...\n");
    make_temp(good_template);
    printf("trying to create second temp file...\n");
    make_temp(bad_template);

    exit(0);
}

void make_temp(char *template)
{
    int fd;
    struct stat sb;

    if ((fd = mkstemp(template)) < 0) {
        err_sys("can't create temp file");
    }
    printf("temp name = %s\n", template);
    close(fd);
    if (stat(template, &sb) < 0) {
        if (errno == ENOENT) {
            printf("file doesn't exists\n");
        } else {
            err_sys("stat failed");
        }
    } else {
        printf("file exists\n");
        unlink(template);
    }
}


/*实验：
[dendi875@localhost ctest]$ ./mkstemp
trying to create first temp file...
temp name = /tmp/dirjbaRSq
file exists
trying to create second temp file...
段错误 (core dumped)


这里要区分数组与指针的区分，看下面的例子

#include <stdio.h>
#include <string.h>

void fun(char *);

int main(void)
{
    //char date1[] = "2019";
    char *date2 = "2019";

    //fun(date1);
    fun(date2);
    //date2 = "20190909";

    //printf("date1 = %s\n", date1);
    printf("date2 = %s\n", date2);

    return 0;
}

void fun(char *date)
{
    strcat(date, "0909");
}*/