/**
 * 非阻塞 write   block write
 */

#include "apue.h"
#include <fcntl.h>
#include <errno.h>

/**
 * 设置文件状态标志（file flags） 启用一个或多个标志位
 */
void set_fl(int fd, int flags)
{
	int 	val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		err_sys("fcntl F_GETFL error");
	}

	val |= flags; /* trun on flags */

	if (fcntl(fd, F_SETFL, val) < 0) {
		err_sys("fcntl F_SETFL ERROR");
	}
}

/**
 * 清除（clr-clear）文件状态标志，关闭一个或多个标志位
 */
void clr_fl(int fd, int flags)
{
	int		val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		err_sys("fcntl F_GETFL error");
	}

	val &= ~flags;	/* trun off flags */

	if (fcntl(fd, F_SETFL, val) < 0) {
		err_sys("fcntl F_SETFL error");
	}
}

char buf[500000];

int main(void)
{
	int 	ntowrite, nwrite;
	char 	*ptr;

	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "read %d bytes\n", ntowrite);

	set_fl(STDOUT_FILENO, O_NONBLOCK);	/* set nonblock */

	ptr = buf;
	while (ntowrite > 0) {
		errno = 0;
		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

		if (nwrite > 0) {
			ptr += nwrite;
			ntowrite -= nwrite;
		}
	}

	clr_fl(STDOUT_FILENO, O_NONBLOCK);

	exit(0);
}

/*
实验：
[dendi875@192 Chapter-14-Advanced IO]$ ./nonblockw < /etc/services  > temp.file
read 500000 bytes
nwrite = 500000, errno = 0

[dendi875@192 Chapter-14-Advanced IO]$ ./nonblockw < /etc/services  2> stderr.out
...（省略）
*/