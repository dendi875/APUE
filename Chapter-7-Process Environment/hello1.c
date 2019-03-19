#include <stdio.h>

main()
{
	printf("hello, world\n");
}

/*实验：
[root@localhost Chapter-7-Process Environment]# ./hello1
hello, world
[root@localhost Chapter-7-Process Environment]# echo $?
13

用C99标准
[root@localhost Chapter-7-Process Environment]# gcc -std=c99 hello1.c -o hello1
hello1.c:4: 警告：返回类型默认为‘int’
[root@localhost Chapter-7-Process Environment]# ./hello1
hello, world
[root@localhost Chapter-7-Process Environment]# echo $?
0



*/