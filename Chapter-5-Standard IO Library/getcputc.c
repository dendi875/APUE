/**
 * 使用getc和putc将标准输入复制到标准输出
 */

#include "apue.h"

int main(void)
{
	int c;

	while ((c = getc(stdin)) != EOF) {
		if (putc(c, stdout) == EOF) {
			err_sys("output error");
		}
	}

	if (ferror(stdin)) {
		err_sys("intput error");
	}

	exit(0);
}


/*实验：
[root@localhost Chapter-5-Standard IO Library]# ./getcputc
123456789
123456789*/