/**
 * getgroups函数使用实例
 */

#include "apue.h"
#define SET_SIZE 5

int main(void)
{
    int i, size;
    gid_t list[SET_SIZE];

    if ((size = getgroups(SET_SIZE, list)) < 0) {
        err_sys("getgroups error");
    }
    for (i = 0; i < size; i++) {
        printf("supplementary group ID：%d\n", list[i]);
    }
    exit(0);
}


/*实验：
[root@localhost ~]# useradd user1
[root@localhost ~]# groupadd group1
[root@localhost ~]# groupadd group2
[root@localhost ~]# groupadd group3

把user1添加到组group1、group2、group3中
[root@localhost ~]# usermod -G group1,group2,group3 user1

为user1设置密码
[root@localhost ~]# passwd user1

切换到user1
[root@localhost ~]# su - user1

查看user1用户的所属组
[user1@localhost ~]$ groups
user1 group1 group2 group3

查看进程所属用户
[user1@localhost ctest]$ ls -l my_getgroups
-rwxrwxr-x 1 user1 user1 10229 3月  15 13:12 my_getgroups

运行程序：
[user1@localhost ctest]$ ./my_getgroups
supplementary group ID：507
supplementary group ID：508
supplementary group ID：509
supplementary group ID：510

验证user1的所属各组的ID是否与程序运行结果一致
[user1@localhost ctest]$ grep user1 /etc/group
user1:x:507:
group1:x:508:user1
group2:x:509:user1
group3:x:510:user1*/