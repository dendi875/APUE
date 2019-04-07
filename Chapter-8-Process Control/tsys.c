/**
 * 使用system执行命令行参数
 */

#include "apue.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void pr_exit(int);
int my_system(const char *);

int main(int argc, char *argv[])
{
	int status;

	if (argc < 2) {
		err_quit("command-line argument required");
	}

	if ((status = my_system(argv[1])) < 0) {
		err_sys("my_system() error");
	}
	pr_exit(status);

	exit(0);
}

int my_system(const char *cmdstring)
{
    pid_t   pid;
    int     status;

    if (cmdstring == NULL) {
        return(1);
    }

    if ((pid = fork()) < 0) {
        status = -1;    /* fork失败 */
    } else if (pid == 0) {  /* 子进程中 */
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127); /* execl失败 */
    } else {    /* 父进程中 */
        while (waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
    }

    return(status);
}

void pr_exit(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termination, signal number = %d%s\n",
            WTERMSIG(status),
#ifdef WCOREDUMP
            WCOREDUMP(status) ? "（core file generated）" : "");
#else
            "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stoped, signal number = %dn", WSTOPSIG(status));
    }
}

/*[dendi875@192 ctest]$ ll
总用量 16
-rwxrwxr-x. 1 dendi875 dendi875 6623 3月  25 21:34 printuids
-rwxrwxr-x. 1 dendi875 dendi875 7331 4月   7 15:14 tsys

[dendi875@192 ctest]$ ./printuids tsys
real uid = 501, effective uid = 501

切换成超级用户
[dendi875@192 ctest]$ su - root

修改所有者为root，并设置SUID位
[root@192 ctest]# chown root tsys
[root@192 ctest]# chmod u+s tsys
[root@192 ctest]# ll
总用量 16
-rwxrwxr-x. 1 dendi875 dendi875 6623 3月  25 21:34 printuids
-rwsrwxr-x. 1 root     dendi875 7331 4月   7 15:14 tsys

切换回普通用户
[root@192 ctest]# exit

[dendi875@192 ctest]$ ./tsys ./printuids
real uid = 501, effective uid = 501
normal termination, exit status = 0
*/
