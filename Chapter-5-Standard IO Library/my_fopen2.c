/**
 * fopen函数实例; 以w方式创建一个新文件，看下创建的新文件的访问权限是什么
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	FILE *fp;

	if (argc != 2) {
		printf("usage：my_fopen2 <filename>\n");
		exit(EXIT_FAILURE);
	}
	// 以写的方式打开，如果文件不存在则创建，如果文件存在则截断文件长度为0
	if ((fp = fopen(argv[1], "w")) == NULL) {
		perror("fopen error");
		exit(EXIT_FAILURE);
	}
	printf("open successfully\n");

	return 0;
}

/*实验：
获取默认的umask值
[root@localhost Chapter-5-Standard IO Library]# umask
0022

创建新的文件
[root@localhost Chapter-5-Standard IO Library]# ./my_fopen2 /test/c_exercise/testfile
open successfully

查看创建文件的权限 666 - 022
[root@localhost Chapter-5-Standard IO Library]# ll /test/c_exercise/testfile
-rw-r--r--. 1 root root 0 3月  11 23:15 /test/c_exercise/testfile

修改umask值
[root@localhost Chapter-5-Standard IO Library]# umask 002
再创建一个新的文件
[root@localhost Chapter-5-Standard IO Library]# ./my_fopen2 /test/c_exercise/testfile2
open successfully

查看新文件的权限 666 - 002 = 664
[root@localhost Chapter-5-Standard IO Library]# ll /test/c_exercise/testfile2
-rw-rw-r--. 1 root root 0 3月  11 23:22 /test/c_exercise/testfile2
*/