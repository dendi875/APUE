/**
 * freopen函数实例；为打开的流附加文件
 * 最常见的用法是把文件和一个标准流相关联
 * 如果流已经打开，则先关闭该流．若流已定向，则使用freopen清除该定向．
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage：my_freopen\n");
		exit(EXIT_FAILURE);
	}
	if (freopen(argv[1], "w", stdout) == NULL) {
		perror("cant't bu opend");
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "open successfully\n");

	exit(0);
}

/*实验：
[root@localhost Chapter-5-Standard IO Library]# ll  /test/c_exercise/testfile
ls: 无法访问/test/c_exercise/testfile: 没有那个文件或目录

注意这里标准输出流已经为打开的文件了，输出的东西不会显示在终端上，而上写到了文件里
[root@localhost Chapter-5-Standard IO Library]# ./my_freopen /test/c_exercise/testfile

[root@localhost Chapter-5-Standard IO Library]# cat /test/c_exercise/testfile
open successfully*/