/**
 * telldir使用实例
 */

#include "apue.h"
#include <dirent.h>

int main(int argc, char *argv[])
{
	DIR *dp;
	long n;
	struct dirent *dirp;

	if (argc != 2) {
		err_quit("usage：my_telldir <directory_name>");
	}
	if ((dp = opendir(argv[1])) == NULL) {
		err_sys("opendir %s error", argv[1]);
	}

	// 打印当前目录的偏移量
	if ((n = telldir(dp)) < 0) {
		err_sys("telldir error");
	}
	printf("current directory offset：%ld\n", n);


	while ((dirp = readdir(dp)) != NULL) {
		printf("name：%-10s\t", dirp->d_name);
		if ((n = telldir(dp)) < 0) {
			err_ret("telldir error");
		} else {
			printf("current directory offset：%ld\n", n);
		}
	}

	closedir(dp);

	exit(0);
}

/*实验：
查看测试目录的文件
[root@localhost Chapter-4-Files and Directories]# ll -ai /test/c_exercise/
总用量 12
262418 drwxr-xr-x. 3 root root 4096 3月  10 11:43 .
786582 drwxr-xr-x. 3 root root 4096 3月   8 23:20 ..
280704 drwxr-xr-x. 2 root root 4096 3月  10 11:43 testdir
262420 -rw-r--r--. 1 root root    0 3月  10 10:47 testfile1
280350 -rw-r--r--. 1 root root    0 3月  10 11:33 testfile2

运行程序
[root@localhost Chapter-4-Files and Directories]# ./my_telldir /test/c_exercise/
current directory offset：0
name：..                current directory offset：1304043065
name：.                 current directory offset：1671350083
name：testfile1         current directory offset：1914086997
name：testdir           current directory offset：2075398182
name：testfile2         current directory offset：2147483647*/