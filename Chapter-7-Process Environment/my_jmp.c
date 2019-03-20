#include "apue.h"
#include <setjmp.h>

jmp_buf env;

void f1(void);
void f2(void);

int main(void)
{
    if (setjmp(env) == 0) {
        printf("setjmp returned 0\n");
    } else {
        printf("Program terminates：longjmp called\n");
        return 0;
    }

    f1();
    printf("Program terminates normally\n");
    return 0;
}

void f1(void)
{
    fprintf(stdout, "f1 begins\n");\
    f2();
    fprintf(stdout, "f1 returns\n");
}

void f2(void)
{
    fprintf(stdout, "f2 begins\n");
    longjmp(env, 1);
    fprintf(stdout, "f2 returns\n");
}


/*实验：
[dendi875@localhost Chapter-7-Process Environment]$ ./my_jmp
setjmp returned 0
f1 begins
f2 begins
Program terminates：longjmp called

第一次调用setjmp时返回0，随后，longjmp将控制权重新转给setjmp宏调用，而setjmp在这次宏调用时会返回
longjmp的第二个参数值。*/