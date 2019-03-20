#include "apue.h"

#define KW "MY_NAME"

int main(void)
{
    char *p;

    if (setenv(KW, "my name is a dendi875", 1) < 0) {
        err_sys("setenv error");
    }
    if ((p = getenv(KW)) == NULL) {
        err_sys("getenv error");
    }
    printf("%s\n", p);

    return 0;
}

/*实验：
[dendi875@localhost Chapter-7-Process Environment]$ ./my_setenv
my name is a dendi875
[dendi875@localhost Chapter-7-Process Environment]$ set | grep MY_NAME

为什么set里没有MY_NAME变量？因为my_setenv程序是由shell进程fork出来的子进程调用exec函数族把子进程
的虚拟内存中的正文段（代码码）、数据段（初始化数据段、bbs段）、堆、栈等替换为my_setenv的程序的。
所以MY_NAME这个环境变量是被设置到了my_setenv进程中，所以在my_setenv进程的父进程（shell）中运行set
是看不到的*/