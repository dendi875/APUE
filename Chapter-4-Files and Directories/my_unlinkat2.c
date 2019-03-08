/**
 * unlinkat() 相对于当前工作目录删除文件
 */

#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		err_quit("usage：my_unlinkat2 <file_name>");
	}
	if (unlinkat(AT_FDCWD, argv[1], 0) < 0) {
		err_sys("unlinkat error for %s：", argv[1]);
	}

	printf("%s unlinked\n", argv[1]);

	exit(0);
}

/**
 * 实验：
 * [root@localhost Chapter-4-Files and Directories]# echo 'hello' > log.txt
 * [root@localhost Chapter-4-Files and Directories]# ./my_unlinkat2 log.txt
 * log.txt unlinked
 */