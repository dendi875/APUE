/**
 * symlink函数使用实例；并不要求第一个参数存在．
 * 1.使用symlink创建一个软链接(源文件不存在)
 * 2.使用lstat获取文件信息，判断是否是一个软链接
 */

#include "apue.h"

int main(void)
{
	struct stat buf;
	char *ptr;

	if (symlink("/no/such/file", "myfile") < 0) {
		err_sys("symlink error");
	}
	printf("created success\n");

	if (lstat("myfile", &buf) < 0) {
		err_sys("lstat error");
	}
	if (S_ISLNK(buf.st_mode)) {
		ptr = "symbolic link";
	} else {
		ptr = "not symbolic link";
	}

	printf("%s\n", ptr);

	exit(0);
}

/**
 * 实验：
[root@localhost c_exercise]# ./my_symlink2
created success
symbolic link
[root@localhost c_exercise]# ll myfile
lrwxrwxrwx. 1 root root 13 3月   8 23:46 myfile -> /no/such/file
 */