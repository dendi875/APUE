/**
 * 打印进程的有效用户ID，实际用户ID
 */

#include "apue.h"

int main(void)
{
	printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());

	exit(0);
}