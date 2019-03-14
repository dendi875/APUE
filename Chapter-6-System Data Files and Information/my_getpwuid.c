/**
 * getpwuid函数使用实例
 */

#include "apue.h"
#include <pwd.h>

int main(void)
{
    struct passwd *ptr;

    if ((ptr = getpwuid(getuid())) == NULL) {
        err_sys("getpwuid error");
    }

    printf("Username = %s\n", ptr->pw_name);
    printf("Passwd = %s\n", ptr->pw_passwd);
    printf("User ID = %d\n", ptr->pw_uid);
    printf("Group ID = %d\n", ptr->pw_gid);
    printf("Home directory = %s\n", ptr->pw_dir);
    printf("Shell program = %s\n", ptr->pw_shell);

    endpwent();

    exit(0);
}


/*实验：
[dendi875@localhost ctest]$ id
uid=504(dendi875) gid=505(dendi875) 组=505(dendi875)

运行程序：
[dendi875@localhost ctest]$ ./my_getpwuid
Username = dendi875
Passwd = x
User ID = 504
Group ID = 505
Home directory = /home/dendi875
Shell program = /bin/bash

[dendi875@localhost ctest]$ grep dendi875 /etc/passwd
dendi875:x:504:505::/home/dendi875:/bin/bash*/