#include "apue.h"
#include <setjmp.h>

static void f1(int, int, int, int);
static void f2(void);

static jmp_buf jmpbuffer;
static int globval; /* 全局变量 */

int main(void)
{
    static int statval;     /* 静态变量 */
    int autoval;            /* 自动变量 */
    register int regival;   /* 寄存器变量 */
    volatile int volaval;   /* 易失变量 */

    globval = 1;
    statval = 2;
    autoval = 3;
    regival = 4;
    volaval = 5;

    if (setjmp(jmpbuffer) != 0) {
        printf("after longjmp：\n");
        printf("globval = %d, statval = %d, autoval = %d, "
               "regival = %d, volaval = %d\n", globval, statval, autoval, regival, volaval);
        exit(0);
    }

    globval = 95;
    statval = 96;
    autoval = 97;
    regival = 98;
    volaval = 99;

    f1(statval, autoval, regival, volaval);
    exit(0);
}

static void f1(int i, int j, int k, int l)
{
    printf("in f1()：\n");
    printf("globval = %d, statval = %d, autoval = %d, regival = %d, "
           "volaval = %d\n", globval, i, j, k, l);
    f2();
}

static void f2(void)
{
    longjmp(jmpbuffer, 1);
}


/*实验：
[dendi875@localhost Chapter-7-Process Environment]$ ./testjmp
in f1()：
globval = 95, statval = 96, autoval = 97, regival = 98, volaval = 99
after longjmp：
globval = 95, statval = 96, autoval = 97, regival = 98, volaval = 99

进行优化后
[dendi875@localhost Chapter-7-Process Environment]$ gcc -I../ -O  -g testjmp.c -o testjmp
[dendi875@localhost Chapter-7-Process Environment]$ ./testjmp
in f1()：
globval = 95, statval = 96, autoval = 97, regival = 98, volaval = 99
after longjmp：
globval = 95, statval = 96, autoval = 3, regival = 4, volaval = 99


全局变量和静态变量在执行longjmp后保持不变。

优化前5种类型变量都放在内存中。
优化后volatile变量仍在内存中。
优化后auto变量和register变量放到了寄存器中。
*/

