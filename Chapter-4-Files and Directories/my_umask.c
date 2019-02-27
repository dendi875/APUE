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
 * 0xff 0XFF
 *
 *八进制转二进制：取1分三
 *S_IRUSR 在/usr/include/bits/stat.h  定义为八进制的 0400　二进制：100 000 000
 *S_IWUSR 在/usr/include/bits/stat.h  定义为八进制的 0200  二进制：010 000 000
 *S_IXUSR 在/usr/include/bits/stat.h  定义为八进制的 0100  二进制：001 000 000
 *
 * #define S_IRGRP (S_IRUSR >> 3) 右移３位，左边补三个0，		   000 100 000
 * #define S_IWGRP (S_IWUSR >> 3)								   000 010 000
 * #define S_IXGRP (S_IXUSR >> 3)								   000 001 000
 * #define S_IROTH (S_IRGRP >> 3)								   000 000 100
 * #define S_IWOTH (S_IWGRP >> 3)								   000 000 010
 * #define S_IXOTH (S_IXGRP >> 3)								   000 000 001
 *
 * S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
 * 按位或后二进制表示为：110 110 110
 * 八进制表示为：666
 * 十进制表示为：438
 *
 * S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
 * 按位或后二进制表示为：000 110 110
 * 八进制表示为：066
 * 十进制表示为：54
 *
 * creat() 函数等价于 open(const char *pathname, O_WRONLY | O_CREAT | O_TRUNCT, mode_t mode)
 *
 * 注意：这里的程序有点费解，书上说的不太明白，其实creat()第二个参数mode和open()第三个参数mode
 * 并不是我们我们入参所指定的真正权限，而是需要通过运算得到mode & (~umask)。
 * 比如上面程序的例子：
 * 1、umask(O)；时creat("foo", RWRWRW)；create参数真正权限为110 110 110 & (~000 000 000)：110 110 110
 * 所以foo文件权限为666。
 * 2、umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);时creat("bar", RWRWRW)参数真正权限为
 * 110 110 110 & (~000 110 110)：110 000 000
 * 所以bar文件权限为600
 * 3、如果注释umask(O)这行代码；则默认的umask为022，create参数真正的权限为
 * 110 110 110 & (~000 010 010)：110 100 100
 * 则foo文件权限644
 */


