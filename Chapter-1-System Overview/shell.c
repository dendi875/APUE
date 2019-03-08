/**
 * 进程控制，从标准输入读命令并执行
 */
#include "apue.h"
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t pid;
    int status;
    char buf[MAXLINE];

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = 0;
        }

        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) {
            execlp(buf, buf, (char *)NULL);
            err_ret("couldn't execute：%s", buf);
            exit(127);
        }

        // 父进程执行逻辑
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            err_sys("waitpid error");
        }
        printf("%% ");
    }

    exit(0);
}

/**
*实验：
* [dendi875@localhost Chapter-1-System Overview]$ ./shell
* % date
* 2019年 03月 07日 星期四 10:05:51 CST
* % pwd
* /mnt/hgfs/www/c_exercise/APUE/Chapter-1-System Overview
*/
