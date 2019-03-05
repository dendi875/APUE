/**
 * unlinkat() 相对目录删除文件实例
 */

#include "apue.h"
#include <fcntl.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	DIR *dp;
	int fd;

	if (argc != 3) {
		err_quit("usage：my_unlinkat <directory_name> <file_name>");
	}
	if ((dp = opendir(argv[1])) == NULL) {
		err_sys("can't open %s：",argv[1]);
	}
	if ((fd = dirfd(dp)) < 0) {
		err_sys("dirfd error");
	}
	if (unlinkat(fd, argv[2], 0) < 0) {
		err_sys("unlinkat error for %s：", argv[2]);
	}
	closedir(dp);

	printf("%s unlinked\n", argv[2]);

	exit(0);
}

/**
 *unlinkat第三个参数为0则删除文件，为AT_REMOVEDIR则删除目录
 *
 *测试：
 *[root@localhost Chapter-4-Files and Directories]# mkdir testdir
 *[root@localhost Chapter-4-Files and Directories]# touch testdir/testfile
 *[root@localhost Chapter-4-Files and Directories]# echo "hello" > !$
 *
 *[root@localhost Chapter-4-Files and Directories]# ./my_unlinkat testdir testfile
 *testfile unlinked
 *
 */