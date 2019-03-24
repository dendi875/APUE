/**
 * 该程序做以下几件事情：
 * 1、使用open创建一个文件
 * 2、使用fstat获取文件用户ID和文件组ID
 * 3、使用geteuid，getuid，getegid，getgid获取文件有有效用户ID，实际用户ID，有效组ID，实际组ID
 * 4、验证书本上第78页上说的：
 * 新建文件的用户ID为进程有有效用户ID，进程的有效用户ID等于进程的实际用户ID（也就是这三个值相等）
 * 新建文件的组ID为进程有有效组ID，进程的有效组ID等于进程的实际组ID（也就是这三个值相等）
 */

#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	int suid, euid, uid, sgid, egid, gid;
	struct stat sb;

	if (argc != 2) {
		err_quit("usage：my_fstat <filename>");
	}

	if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
		err_sys("open error");
	}

	if (fstat(fd, &sb) < 0) {
		err_sys("fstat error");
	}

	// 打印文件所有者ID，进程有效用户ID，进程实际用户ID
	printf("文件所有者ID为：%d, 进程有效组ID为：%d, 进程实际组ID为：%d\n",
		 	sb.st_uid, geteuid(), getuid());
	// 打印文件所属组ID，进程有效组ID，进程实际组ID
	printf("文件所属组ID为：%d, 进程有效组ID为：%d, 进程实际组ID为：%d\n",
			sb.st_gid, getegid(), getgid());

	exit(0);	/* 当一个进程终止时，内核会关闭该进程所有打开的文件 */
}

/*实验：
[dendi875@localhost ctest]$ ./my_fstat3 fstat3.txt
文件所有者ID为：501, 进程有效组ID为：501, 进程实际组ID为：501
文件所属组ID为：501, 进程有效组ID为：501, 进程实际组ID为：501*/