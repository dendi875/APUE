/**
 * 创建多个进程，然后使用kill -9 -进程组ID　杀死一个进程组中所有进程
 */

#include "apue.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t   pid;
    int     i, n;

    if (argc != 2) {
        err_quit("usage：./my_kill_multprocess.c <childnum>");
    }

    n = atoi(argv[1]);
    for (i = 0; i < n; i++) {
        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) {
            break;
        }
    }

    if (pid == 0) {     /* 子进程中 */
        while (1) {
            printf("I'm child pid = %d, pgid = %d\n", getpid(), getpgid(0));
            sleep(1);
        }
    }

    if (pid > 0) {      /* 父进程中 */
        pid_t   cpid;
        while (1) {     /* 轮询（polling）回收子进程 */
            if ((cpid = waitpid(-1, NULL, 0)) > 0) {    /* 阻塞等待任一子进程结束 */
                printf("child %d id exit\n", cpid);
            } else {
                err_sys("waitpid error");
            }
        }
    }

    exit(0);
}

/*
实验：
[dendi875@localhost ~]$ ps ajxf
8304  8305  8305  8305 pts/1     8412 Ss     504   0:00          \_ -bash
8305  8412  8412  8305 pts/1     8412 S+     504   0:00          |   \_ ./my_kill_multprocess 5
8412  8413  8412  8305 pts/1     8412 S+     504   0:00          |       \_ ./my_kill_multprocess 5
8412  8414  8412  8305 pts/1     8412 S+     504   0:00          |       \_ ./my_kill_multprocess 5
8412  8415  8412  8305 pts/1     8412 S+     504   0:00          |       \_ ./my_kill_multprocess 5
8412  8416  8412  8305 pts/1     8412 S+     504   0:00          |       \_ ./my_kill_multprocess 5
8412  8417  8412  8305 pts/1     8412 S+     504   0:00          |       \_ ./my_kill_multprocess 5

[dendi875@localhost ~]$ kill -15 -8412
*/