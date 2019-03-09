/**
 * symlinkat函数使用实例
 */

#include "apue.h"
#include <dirent.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	DIR *dp;
	int fd;
	struct stat sb;
	char *ptr;

	if (argc != 4) {
		err_quit("usage：my_symlinkat <oldfile> <dirname> <newfile>");
	}
	if ((dp = opendir(argv[2])) == NULL) {
		err_sys("opendir %s error", argv[2]);
	}
	if ((fd = dirfd(dp)) < 0) {
		err_sys("dirfd error");
	}
	printf("fd = %d\n", fd);

	if (symlinkat(argv[1], fd, argv[3]) < 0) {
		err_sys("symlinkat error");
	}
	printf("symbolic link %s created success\n", argv[3]);

	if (fstatat(fd, argv[3], &sb, AT_SYMLINK_NOFOLLOW) < 0) {
		err_sys("fstatat error");
	}
	if (S_ISLNK(sb.st_mode)) {
		ptr = "symbolic link";
	} else {
		ptr = "not symbolic link";
	}

	printf("%s\n", ptr);

	exit(0);
}


/**
实验：
[root@localhost c_exercise]# mkdir -p test1/test11
[root@localhost c_exercise]# ./my_symlinkat /etc/passwd test1/test11 passwd-so
fd = 3
symbolic link passwd-so created success
symbolic link
 */