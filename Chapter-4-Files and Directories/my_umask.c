#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(int argc, char *argv[])
{
	umask(0);

	if (creat("foo", RWRWRW) < 0) {
		err_sys("creat error for foo");
	}
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (creat("bar", RWRWRW) < 0) {
		err_sys("creat error for bar");
	}

	exit(0);
}

/**
 * C语言中
 * 十进制数常量包含0~9中的数字，但是一定不能以零开头     11  111  1111
 * 八进制数常量包含0~7中的数字，而且必须以零开头         017  0377 07777
 * 十六进制数常量包含0~9中的数字和a~f中的字母，而且总是以0x开头，十六进制中的字母即可以是大写
 * 字母也可以是小写的
 * 0xff OXFF
 *
 *八进制转二进制：取1分三
 *S_IRUSR 在/usr/include/bits/stat.h  定义为八进制的 0400　二进制：000 100 000 000
 *S_IWUSR 在/usr/include/bits/stat.h  定义为八进制的 0200  二进制：000 010 000 000
 *S_IXUSR 在/usr/include/bits/stat.h  定义为八进制的 0100  二进制：000 001 000 000
 *
 * #define S_IRGRP (S_IRUSR >> 3) 右移３位，左边补三个0，		   000 000 100 000
 * #define S_IWGRP (S_IWUSR >> 3)								   000 000 010 000
 * #define S_IXGRP (S_IXUSR >> 3)								   000 000 001 000
 * #define S_IROTH (S_IRGRP >> 3)								   000 000 000 100
 * #define S_IWOTH (S_IWGRP >> 3)								   000 000 000 010
 * #define S_IXOTH (S_IXGRP >> 3)								   000 000 000 001
 *
 * creat() 函数等价于 open(const char *pathname, O_WRONLY | O_CREAT | O_TRUNCT, mode_t mode)
 *
 * S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH 按位或后值为：000 110 110 110 转为十进制为：0666
 * S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH 按位或后值为：000 000 110 110  转为十进制为：0066
 *
 */