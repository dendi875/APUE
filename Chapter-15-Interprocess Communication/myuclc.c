/**
 * 将标准输入复制到标准输出，在复制时将大写字符变换为小写字符
 */

#include "apue.h"
#include <ctype.h>

int main(void)
{
    int c;

    while ((c = getchar()) != EOF) {
        if (isupper(c)) {
            c = tolower(c);
        }
        if (putchar(c) == EOF) {
            err_sys("output error");
        }
        if (c == '\n') {
            fflush(stdout);
        }
    }

    exit(0);
}