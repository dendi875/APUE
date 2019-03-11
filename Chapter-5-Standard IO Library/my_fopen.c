/**
 * fopen函数实例；以r(只读方式)打开一个文件，如果文件不存在则出错
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	FILE *fp;

	if (argc != 2) {
		printf("usage：my_fopen <filename>\n");
		exit(EXIT_FAILURE);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		perror("fopen error");
		exit(EXIT_FAILURE);
	}
	printf("open successfully\n");

	fclose(fp);

	return 0;
}

/*实验：
[root@localhost Chapter-5-Standard IO Library]# ./my_fopen /test/c_exercise/example.dat
fopen error: No such file or directory

[root@localhost Chapter-5-Standard IO Library]# touch !$
touch /test/c_exercise/example.dat

[root@localhost Chapter-5-Standard IO Library]# ll !$
ll /test/c_exercise/example.dat
-rw-r--r--. 1 root root 0 3月  11 23:08 /test/c_exercise/example.dat

[root@localhost Chapter-5-Standard IO Library]# ./my_fopen /test/c_exercise/example.dat
open successfully*/