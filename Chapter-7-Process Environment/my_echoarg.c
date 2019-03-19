/**
 * echoarg的别一个版本（指向字符的指针的指针）
 * argv[argc]为NULL
 */
#include "apue.h"

int main(int argc, char **argv)
{
	int i = 0;
	char **p;

	for (p = argv; *p != NULL; p++) {
		printf("argv[%d]: %s\n", i++, *p);
	}
	exit(0);
}

// 实验：
// [root@localhost Chapter-7-Process Environment]# ./my_echoarg arg1 TEST foo
// argv[0]: ./my_echoarg
// argv[1]: arg1
// argv[2]: TEST
// argv[3]: foo