/**
 * 信号集相关操作函数实例
 */

#include "apue.h"
#include <signal.h>

static void pr_sigset(sigset_t *);

int main(void)
{
    sigset_t myset;

    sigemptyset(&myset);
    sigaddset(&myset, SIGHUP);
    sigaddset(&myset, SIGINT);
    sigaddset(&myset, SIGQUIT);
    sigaddset(&myset, SIGUSR1);
    sigaddset(&myset, SIGKILL);
    sigaddset(&myset, SIGSTOP);

    pr_sigset(&myset);

    return 0;
}

static void pr_sigset(sigset_t *set)
{
    int i;

    for (i = 1; i < _NSIG; i++) {
        if (sigismember(set, i)) {
            fputc('1', stdout);
        } else {
            fputc('0', stdout);
        }
    }
    putchar('\n');
}

/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./my_sigset2
1110000011000000001000000000000000000000000000000000000000000000

信号集被定义为一种数据类型：
typedef unsigned long sigset_t;

信号集用来描述信号的集合，每个信号占用一位（64位）。Linux所支持的所有信号可以全部或部分的出现在信号集中，主要与信号阻塞相关函数配合使用。下面是信号集操作相关的函数：

sigemptyset(sigset_t *set) 清空信号集中所有信号，相当于64位都置为0；
sigfillset(sigset_t *set) 信号集中包含linux支持的64种信号，相当于64位都置为1；
sigaddset(sigset_t *set, int signum) 将一个信号添加到信号集中，相当于将给定信号所对应的位置为1；
sigdelset(sigset_t *set, int signum) 从信号集中删除一个信号，相当于将给定信号所对应的位置为0；
sigismember(const sigset_t *set, int signum) 判断某个信号是否在信号集中，相当于检查给定信号所对应的位是0还是1。

注意这5个函数只是控制将信号添加到信号集中，或者从信号集中删除。有了这个信号集，就可以用这个信号集来调用
sigprocmask函数

*/
