/**
 *在多进程模型中，每个进程只有一个线程。这与在多线程模型中每个进程只包含一个线程是相同的。
 */

#include "apue.h"
#include <pthread.h>

int main(void)
{
    pid_t       pid;
    pthread_t   tid;    /* linux使用无符号长整型表示pthread_t数据类型 */

    pid = getpid();
    tid = pthread_self();   /* 获取当前线程的线程ID */

    printf("main thread：pid %lu tid %lu (0x%lx)\n", (unsigned long)pid, (unsigned long)tid,
            (unsigned long)tid);

    exit(0);
}

/*注意gcc时要加上-pthread选项
实验：
[dendi875@localhost Chapter-11-Threads]$ ./my_pthread_self
main thread：pid 8981 tid 3077555904 (0xb76fc6c0)

*/