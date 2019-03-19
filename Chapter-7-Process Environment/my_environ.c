/**
 * 全局变量环境指针 char **environ的实例
 */
#include "apue.h"

extern char **environ;

int main(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++) {
		printf("%s\n", environ[i]);
	}
	exit(0);
}

/*实验：
[root@localhost Chapter-7-Process Environment]# ./my_environ
HOSTNAME=localhost.localdomain
SELINUX_ROLE_REQUESTED=
TERM=linux
SHELL=/bin/bash
HISTSIZE=1000
...
...
PWD=/data/www/c_exercise/APUE/Chapter-7-Process Environment
LANG=zh_CN.UTF-8
SELINUX_LEVEL_REQUESTED=
HISTCONTROL=ignoredups
SHLVL=1
HOME=/root

程序执行结果与env命令显示的一样*/