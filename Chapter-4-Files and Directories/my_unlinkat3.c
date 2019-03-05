/**
 * 相对于当前工作目录删除目录
 */

#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
	struct stat buf;

	if (argc != 2) {
		err_quit("usage：my_unlinkat3 <directory_name>");
	}
	if (lstat(argv[1], &buf) < 0) {
		err_sys("lstat error");
	}
	if (!S_ISDIR(buf.st_mode)) {
		err_quit("%s not is directory", argv[1]);
	}
	if (unlinkat(AT_FDCWD, argv[1], AT_REMOVEDIR) < 0) {
		err_sys("unlinkat error");
	}
	printf("%s unlinked\n", argv[1]);

	exit(0);
}


/**
 * 测试：
 * [root@localhost Chapter-4-Files and Directories]# ./my_unlinkat3 testdir
 * testdir unlinked
 */