/**
 * ungetc函数实例
 */

#include "apue.h"
#include <ctype.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	int ch;

	if (argc != 2) {
		err_quit("usage：my_ungetc <filename>");
	}
	if ((fp = fopen(argv[1], "r+")) == NULL) {
		err_sys("fopen error %s", argv[1]);
	}

	// 从文件中读入一系列数字，遇到首个非数字时停止
	while (isdigit(ch = getc(fp))) {
		putc(ch, stdout);
	}
	ungetc(ch, fp);

	putchar('\n');
	putc(getc(fp), stdout);
	putchar('\n');

	fclose(fp);
	exit(0);
}


/*实验：
不加ungetc(ch, fp);
[root@localhost Chapter-5-Standard IO Library]# echo '1234ab5678' > /test/c_exercise/bar
[root@localhost Chapter-5-Standard IO Library]# ./my_ungetc /test/c_exercise/bar
1234
b

加上ungetc(ch, fp);
[root@localhost Chapter-5-Standard IO Library]# ./my_ungetc /test/c_exercise/bar
1234
a*/

