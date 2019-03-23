/**
 * execl函数实例
 */

#include "apue.h"

int main(void)
{
    printf("hello\n");
    if (execl("/bin/ls", "ls", "-l", NULL) < 0) {
        err_sys("execl error");
    }

/*    if (execl("/bin/ls", "aaaa", "-l", NULL) < 0) {
        err_sys("execl error");
    }

    if (execl("/bin/ls", "", NULL) < 0) {
        err_sys("execl error");
    }*/

    printf("world\n");
    exit(0);
}



/*实验：
[dendi875@localhost Chapter-8-Process Control]$ ./my_execl
hello
总用量 62
-rwxrwxrwx 1 root root 10833 3月  21 19:54 fork1
-rwxrwxrwx 1 root root  3487 3月  22 16:44 fork1.c
-rwxrwxrwx 1 root root  7042 3月  23 16:24 fork2
-rwxrwxrwx 1 root root  1512 3月  23 16:25 fork2.c
-rwxrwxrwx 1 root root  6527 3月  23 18:24 my_execl
-rwxrwxrwx 1 root root   230 3月  23 18:23 my_execl.c
-rwxrwxrwx 1 root root 10721 3月  21 20:37 my_fork
-rwxrwxrwx 1 root root  1317 3月  22 17:02 my_fork.c
-rwxrwxrwx 1 root root 11229 3月  23 15:32 my_wait
-rwxrwxrwx 1 root root  2207 3月  23 15:33 my_wait.c
-rwxrwxrwx 1 root root 11291 3月  23 15:39 my_waitpid
-rwxrwxrwx 1 root root  1506 3月  23 16:25 my_waitpid.c
-rwxrwxrwx 1 root root 10599 3月  23 13:48 my_zombie
-rwxrwxrwx 1 root root  1249 3月  23 14:19 my_zombie.c
-rwxrwxrwx 1 root root     0 2月  25 17:11 README.md
-rwxrwxrwx 1 root root  6721 3月  23 17:30 tellwait1
-rwxrwxrwx 1 root root   752 3月  23 17:30 tellwait1.c
-rwxrwxrwx 1 root root   104 3月  22 16:40 temp.out
-rwxrwxrwx 1 root root    81 3月  22 16:55 test.out
-rwxrwxrwx 1 root root 10582 3月  22 20:02 vfork1
-rwxrwxrwx 1 root root  1505 3月  22 20:05 vfork1.c
-rwxrwxrwx 1 root root 10817 3月  23 12:12 wait1
-rwxrwxrwx 1 root root  2433 3月  23 12:12 wait1.c

我们把execl("/bin/ls", "ls", "-l", NULL)换成execl("/bin/ls", "aaa", "-l", NULL);
[dendi875@localhost Chapter-8-Process Control]$ ./my_execl
hello
总用量 62
-rwxrwxrwx 1 root root 10833 3月  21 19:54 fork1
-rwxrwxrwx 1 root root  3487 3月  22 16:44 fork1.c
-rwxrwxrwx 1 root root  7042 3月  23 16:24 fork2
-rwxrwxrwx 1 root root  1512 3月  23 16:25 fork2.c
-rwxrwxrwx 1 root root  6527 3月  23 18:26 my_execl
-rwxrwxrwx 1 root root   331 3月  23 18:26 my_execl.c
-rwxrwxrwx 1 root root 10721 3月  21 20:37 my_fork
-rwxrwxrwx 1 root root  1317 3月  22 17:02 my_fork.c
-rwxrwxrwx 1 root root 11229 3月  23 15:32 my_wait
-rwxrwxrwx 1 root root  2207 3月  23 15:33 my_wait.c
-rwxrwxrwx 1 root root 11291 3月  23 15:39 my_waitpid
-rwxrwxrwx 1 root root  1506 3月  23 16:25 my_waitpid.c
-rwxrwxrwx 1 root root 10599 3月  23 13:48 my_zombie
-rwxrwxrwx 1 root root  1249 3月  23 14:19 my_zombie.c
-rwxrwxrwx 1 root root     0 2月  25 17:11 README.md
-rwxrwxrwx 1 root root  6721 3月  23 17:30 tellwait1
-rwxrwxrwx 1 root root   752 3月  23 17:30 tellwait1.c
-rwxrwxrwx 1 root root   104 3月  22 16:40 temp.out
-rwxrwxrwx 1 root root    81 3月  22 16:55 test.out
-rwxrwxrwx 1 root root 10582 3月  22 20:02 vfork1
-rwxrwxrwx 1 root root  1505 3月  22 20:05 vfork1.c
-rwxrwxrwx 1 root root 10817 3月  23 12:12 wait1
-rwxrwxrwx 1 root root  2433 3月  23 12:12 wait1.c

我们把execl("/bin/ls", "ls", "-l", NULL)换成execl("/bin/ls", "", NULL)
[dendi875@localhost Chapter-8-Process Control]$ ./my_execl
hello
fork1    fork2    my_execl    my_fork    my_wait    my_waitpid    my_zombie    README.md  tellwait1.c  test.out  vfork1.c  wait1.c
fork1.c  fork2.c  my_execl.c  my_fork.c  my_wait.c  my_waitpid.c  my_zombie.c  tellwait1  temp.out     vfork1    wait1
*/