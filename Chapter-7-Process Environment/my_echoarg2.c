/**
 * echoarg.c 程序的另一个版本实现
 */
#include "apue.h"

int main(int argc, char *argv[])
{
	int i;

	for (i = 0; argv[i] != NULL; i++) {
		printf("argv[%d]: %s\n", i, argv[i]);
	}
	exit(0);
}

// 实验：
// [root@localhost Chapter-7-Process Environment]# ./my_echoarg2 arg1 TEST foo
// argv[0]: ./my_echoarg2
// argv[1]: arg1
// argv[2]: TEST
// argv[3]: foo
