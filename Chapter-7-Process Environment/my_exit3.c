/**
 * 该程序证明如果main函数调用了用户函数，用户函数调用了_exit或_Exit，
 * 那么用户函数直接返回内核，不会再调用atexit注册的函数。
 */
#include "apue.h"

static void my_exit1(void);
static void my_exit2(void);

static void f1(void);

int main(void)
{
    if (atexit(my_exit2) != 0) {
        err_sys("can't register my_exit2");
    }

    if (atexit(my_exit1) != 0) {
        err_sys("can't register my_exit1");
    }
    if (atexit(my_exit1) != 0) {
        err_sys("can't register my_exit1");
    }
    f1();
    printf("main is done\n");
}

static void my_exit1(void)
{
    printf("first exit handler\n");
}

static void my_exit2(void)
{
    printf("second exit handler\n");
}

static void f1(void)
{
    printf("I am f1 function\n");
    _exit(0); // 等价于 _Exit(0);
}


/*实验：
[dendi875@localhost Chapter-7-Process Environment]$ ./my_exit3
I am f1 function

1、atexit是注册程序终止时要调用的函数，可以注册多个。这些注册的函数由exit函数自动调用，调用的顺序与注册时相反，一个函数如果注册多次则被调用多次。
2、调用_exit或_Exit函数会立即进入内核，而exit函数会先自动调用注册到atexit的函数，然后再隐式的调用_exit或_Exit，然后由_exit或_Exit进入内核。_exit与_Exit函数作用相同
3、启动例程的代码类似这样exit(main(argc, argv))，启动例程中的代码调用main函数后，
接着用main函数的返回值当入参调用exit函数。所以如果main函数没有调用exit或_exit或_Exit终止程序，那么
main函数会返回到启动例程，启动例程中的exit执行，exit执行了那么就会调用atexit注册的函数，会隐式调用
_exit或_Exit返回内核。所以在main中return一个整型值与exit调用该整型值是一样的
4、如果程序中调用了exec函数族中的任一函数，则将清除所有已安装的终止处理程序。
*/