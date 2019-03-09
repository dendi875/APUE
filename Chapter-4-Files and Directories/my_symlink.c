/**
 * symlink创建一个文件的软链接
 */

#include "apue.h"

int main(void)
{
	if (symlink("/etc/passwd", "/test/c_exercise/passwd-so") < 0) {
		err_sys("symlink error");
	}
	printf("created success\n");

	exit(0);
}

/**
 * 实验：
 * [root@localhost c_exercise]# du -s .; df -i .
680     .
文件系统              Inode  已用(I)  可用(I) 已用(I)%% 挂载点
/dev/sda5            1105920  129799  976121   12% /
[root@localhost c_exercise]# ./my_symlink
created success
[root@localhost c_exercise]# du -s .; df -i .
680     .
文件系统              Inode  已用(I)  可用(I) 已用(I)%% 挂载点
/dev/sda5            1105920  129800  976120   12% /
[root@localhost c_exercise]# ls -li /etc/passwd passwd-so
150072 -rw-r--r--. 1 root root 1134 12月 31 2017 /etc/passwd
280867 lrwxrwxrwx. 1 root root   11 3月   8 23:26 passwd-so -> /etc/passwd
 */