#include "apue.h"

int main(int argc, char *argv[])
{
	int i;

	for (i = 0; i < argc; i++) {	/* echo all command-line args */
		printf("argv[%d]: %s\n", i, argv[i]);
	}
	exit(0);
}


/*fork有两个典型的用法：
1、一个进程创建一个自身的拷贝，这样每个拷贝都可以在另一个拷贝执行其他任务的同时处理各自的某个操作。这是网络服务器的典型用法。
2、一个进程想要执行另一个程序。既然创建新进程的唯一方法为调用fork，该进程于是首先调用fork创建一个自身的拷贝，然后其中一个拷贝（通常为子进程）调用exec把自身替换成新的程序。这是诸如shell之类程序的典型用法。

当执行一个程序时，调用exec的进程可将命令行参数传递给该新程序。

实验：
[root@localhost Chapter-7-Process Environment]# ./echoarg arg1 TEST foo
argv[0]: ./echoarg
argv[1]: arg1
argv[2]: TEST
argv[3]: foo

argv[argc] 是个空指针
*/