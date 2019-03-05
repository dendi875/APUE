/**
 * open函数使用实例
 */
#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) // 666

int main(void)
{
	umask(0);
	if (open("tempfile", O_RDWR | O_CREAT | O_TRUNC, RWRWRW) < 0) {
		err_sys("open error");
	}

	exit(0);
}

/**
 * 注意:
 * 1.文件不存在的情况下如果第二个参数只有O_RDWR，则出错
 * 2.如果第二个参数中包含了O_CREAT则必须给出第三个参数
 */