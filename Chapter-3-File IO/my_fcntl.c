/**
 * 使用fcntl函数获取文件描述符状态标志
 */

#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd, val;

	if (argc != 2) {
		err_quit("usage：my_fcntl <filename>");
	}

	if ((fd = open(argv[1], O_RDONLY | O_CREAT, FILE_MODE)) < 0) {
		err_sys("open error");
	}

	if ((val = fcntl(fd, F_GETFD, 0)) < 0) {
		err_sys("fcntl F_GETFD error");
	}

	printf("%s 文件描述符标志为：%d\n", argv[1], val);

	exit(0);
}

/*[dendi875@localhost Chapter-3-File IO]$ ./my_fcntl fd.txt
fd.txt 文件描述符标志为：0

文件描述符标志说的是在使用exec函数时当前进程的打开文件描述符是要关闭还是不关闭。
0表示不关闭，1表示关闭。默认是0*/