#include "apue.h"

int main(void)
{
    char *keyword[] = {"PATH", "PWD", "HOME", "PS1", NULL};

    char **kwp;
    char *p;
    for (kwp = keyword; *kwp != NULL; kwp++) {
        if ((p = getenv(*kwp)) != NULL) {
            printf("%s=%s\n", *kwp, p);
        }
    }

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-7-Process Environment]$ ./my_getenv
PATH=/usr/lib/qt-3.3/bin:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin:/usr/local/mysql/bin:/usr/local/php56/bin:/home/dendi875/bin
PWD=/data1/www/c_exercise/APUE/Chapter-7-Process Environment
HOME=/home/dendi875*/