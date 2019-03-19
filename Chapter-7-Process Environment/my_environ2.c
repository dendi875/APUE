/**
 * 环境指针全局变量 char **environ 实例
 */

#include "apue.h"

int main(void)
{
	extern char **environ;

	while (*environ != NULL) {
		printf("%s\n", *environ);
		environ++;
	}
	exit(0);
}

/*实验：
[root@localhost Chapter-7-Process Environment]# ./my_environ2
HOSTNAME=localhost.localdomain
SELINUX_ROLE_REQUESTED=
TERM=linux
SHELL=/bin/bash
HISTSIZE=1000
...
...
LANG=zh_CN.UTF-8
SELINUX_LEVEL_REQUESTED=
HISTCONTROL=ignoredups
SHLVL=1
HOME=/root
LOGNAME=root

结果与env命令显示的一样*/