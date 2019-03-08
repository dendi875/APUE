/**
 * 绝对路径方式删除文件
 */
#include "apue.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		err_quit("usage：my_unlink <pathname>");
	}
	if (unlink(argv[1]) < 0) {
		err_sys("unlink error for %s：", argv[1]);
	}
	printf("file %s unlinked\n", argv[1]);

	exit(0);
}

/**
 * 测试：
 *[root@localhost Chapter-4-Files and Directories]# touch /tmp/testfile
 *[root@localhost Chapter-4-Files and Directories]# ./my_unlink /tmp/testfile
 *[root@localhost Chapter-4-Files and Directories]# ./my_unlink /tmp/testfile
 * file /tmp/testfile unlinked
 */