/**
 * fopen函数以"a"方式打开一个文件的用法
 * "a"代表：以写方式打开，如果文件不存在则创建，文件存在则以追加的方式写数据
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int c;
	FILE *fp;

	if (argc != 2) {
		fprintf(stderr, "usage：my_fopen3 <filename>\n");
		exit(EXIT_FAILURE);
	}
	if ((fp = fopen(argv[1], "a")) == NULL) {
		fprintf(stderr, "fopen error %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	while ((c = getc(stdin)) != EOF) {
		if (putc(c, fp) == EOF) {
			fprintf(stderr, "output error");
			exit(EXIT_FAILURE);
		}
	}

	fclose(fp);

	return 0;
}


/*实验:
向测试文件中写入数据
[root@localhost Chapter-5-Standard IO Library]# echo '01234' > /test/c_exercise/example.dat

运行程序按ctrl+d结束
[root@localhost Chapter-5-Standard IO Library]# ./my_fopen3 /test/c_exercise/example.dat
56789

查看文件内容
[root@localhost Chapter-5-Standard IO Library]# cat /test/c_exercise/example.dat
01234
56789*/