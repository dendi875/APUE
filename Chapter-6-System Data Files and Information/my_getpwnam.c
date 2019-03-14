/**
 * getpwnam函数使用实例
 */

#include "apue.h"
#include <pwd.h>

#define USER_NAME "dendi875"

int main(void)
{
    struct passwd *ptr;

    if ((ptr = getpwnam(USER_NAME)) == NULL) {
        err_sys("getpwnam error");
    }
    printf("Username = %s\n", ptr->pw_name);
    printf("Password = %s\n", ptr->pw_passwd);
    printf("User ID = %d\n", ptr->pw_uid);
    printf("Group ID = %d\n", ptr->pw_gid);
    printf("Home directory = %s\n", ptr->pw_dir);
    printf("Shell program = %s\n", ptr->pw_shell);

    endpwent();

    exit(0);
}

/*查看当前用户ID
[dendi875@localhost Chapter-6-System Data Files and Information]$ id
uid=504(dendi875) gid=505(dendi875) 组=505(dendi875)

运行程序
[dendi875@localhost Chapter-6-System Data Files and Information]$ ./my_getpwnam
Username = dendi875
Password = x
User ID = 504
Group ID = 505
Home directory = /home/dendi875
Shell program = /bin/bash
*/
