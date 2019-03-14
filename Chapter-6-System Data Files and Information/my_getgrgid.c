/**
 * getgrgid()函数使用实例
 */

#include "apue.h"
#include <grp.h>

int main(void)
{
	struct group *gpr;

	if ((gpr = getgrgid(getgid())) == NULL) {
		err_sys("getgrgid error");
	}
	printf("group name = %s\n", gpr->gr_name);
	printf("group ID = %d\n", gpr->gr_gid);

	endgrent();

	exit(0);
}

/*实验：
[zq@localhost ctest]$ groups
zq
[zq@localhost ctest]$ grep zq /etc/group
zq:x:500:
[zq@localhost ctest]$ ./my_getgrgid
group name = zq
group ID = 500*/