/**
 * 测试标准输入能否被设置偏移量
 */
#include "apue.h"

int main(int argc, char **argv)
{
	off_t currpos;

	if ((currpos = lseek(STDIN_FILENO, 0, SEEK_CUR)) == -1) {
		printf("cannot seek\n");
	} else {
		printf("seek OK\n");
	}

	exit(0);
}

/**
 * lseek()中的l字符就代表入参和返回值off_t为loing int类型
 *
 * 实验：
 * [root@localhost Chapter-3-File IO]# ./my_lseek
 * cannot seek
 *
 * [root@localhost Chapter-3-File IO]# ./my_lseek < /etc/passwd
 * seek OK
 *
 * [root@localhost Chapter-3-File IO]# cat < /etc/passwd | ./my_lseek
 * cannot seek
 *
 * cat < /etc/passwd 的结果通过管道符作为 ./my_lseek的输入
 */
