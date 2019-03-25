/**
 * 使用system执行命令行参数
 */

#include "apue.h"

void pr_exit(int);

int main(int argc, char *argv[])
{
	int status;

	if (argc < 2) {
		err_quit("command-line argument required");
	}

	if ((status = system(argv[1])) < 0) {
		err_sys("system() error");
	}
	pr_exit(status);

	exit(0);
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


/*实验：
[dendi875@localhost ctest]$ ll
总用量 16
-rwxrwxr-x. 1 dendi875 dendi875 6623 3月  25 21:40 printuids
-rwxrwxr-x. 1 dendi875 dendi875 6874 3月  25 21:40 tsys
[dendi875@localhost ctest]$ ./tsys ./printuids
real uid = 501, effective uid = 501
normal termination, exit status = 0


切换成root设置SUID
[root@localhost ctest]# chown root tsys
[root@localhost ctest]# chmod 4775 tsys
[root@localhost ctest]# ll tsys
-rwsrwxr-x. 1 root dendi875 6874 3月  25 21:40 tsys
[root@localhost ctest]# exit
logout
[dendi875@localhost ctest]$ ./tsys ./printuids
real uid = 501, effective uid = 501
normal termination, exit status = 0*/

