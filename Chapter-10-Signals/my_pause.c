/**
 * puse函数实例，说明当一个信号到达时默认的动作是不会使pause返回
 */

#include "apue.h"
#include <unistd.h>
#include <errno.h>

int main(void)
{
    pause();
    if (errno == EINTR) {
        printf("pause returned, errno is set to EINTR\n");
    }
    exit(0);
}

/*[dendi875@localhost Chapter-10-Signals]$ ./my_pause &
[1] 20055
[dendi875@localhost Chapter-10-Signals]$ jobs -l
[1]+ 20055 Running                 ./my_pause &
[dendi875@localhost Chapter-10-Signals]$ kill -15 20055*/