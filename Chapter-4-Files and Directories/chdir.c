#include "apue.h"

int main(void)
{
	if (chdir("/tmp") < 0) {
		err_sys("chdir failed");
	}
	printf("chdir to /tmp succeeded\n");
	exit(0);
}


/*实验：
[root@localhost Chapter-4-Files and Directories]# pwd
/data/www/c_exercise/APUE/Chapter-4-Files and Directories
[root@localhost Chapter-4-Files and Directories]# ./chdir
chdir to /tmp succeeded
[root@localhost Chapter-4-Files and Directories]# pwd
/data/www/c_exercise/APUE/Chapter-4-Files and Directories

每个程序运行在独立的进程中，shell的当前工作目录并不会随着程序调用chdir而改变

*/