/**
 * tmpnam函数使用实例
 */

#include "apue.h"

int main(void)
{
	char name[L_tmpnam];

	printf("%s\n", tmpnam(NULL));

	tmpnam(name);
	printf("%s\n", name);

	return 0;
}

/*实验：
编译
[root@localhost Chapter-5-Standard IO Library]# gcc -I../ my_tmpnam.c -o my_tmpnam
/tmp/ccQVXV8X.o: In function `main':
my_tmpnam.c:(.text+0x207): warning: the use of `tmpnam' is dangerous, better use `mkstemp'

运行
[root@localhost Chapter-5-Standard IO Library]# ./my_tmpnam
/tmp/file2ejbPQ
/tmp/filetS1gHS

查看
[root@localhost Chapter-5-Standard IO Library]# ll /tmp/file2ejbP /tmp/filetS1gHS
ls: 无法访问/tmp/file2ejbP: 没有那个文件或目录
ls: 无法访问/tmp/filetS1gHS: 没有那个文件或目录

可以看到tmpnam函数只是产生一个唯一的有效路径名字字符串*/