/**
 *信号设置和sigprocmask实例
 */

#include "apue.h"
#include <signal.h>

static void sig_quit(int);

int main(void)
{
    sigset_t    newmask, oldmask, pendmask;

    if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
        err_sys("can't catch SIGQUIT");
    }

    /**
     *使当前进程阻塞SIGQUIT信号，并保存原来的阻塞信号集
     */
    sigemptyset(&newmask);          /* 清空信号集中的所有信号 */
    sigaddset(&newmask, SIGQUIT);   /* 把SIGQUIT信号加入到信号集中 */
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) { /* 使用我们构造的信号集+sigprocmask函数来更改当前进程的阻塞信号集 */
        err_sys("SIG_BLOCK error");
    }

    /**
     *在这5秒钟之内产生的SIGQUIT信号是未决的（未决信号集中对应位值为1），阻塞的（阻塞信号集中对应位值为1）
     *所以SIGQUIT信号不会被递送给该进程。该进程不捕捉不到SIGQUIT信号，不会调用注册的信号处理函数sig_quit
     */
    sleep(5);

    /**
     *返回一个信号集，该信号集中的信号是未决的，阻塞的。结果存储到pendmask指向的信号集中
     */
    if (sigpending(&pendmask) < 0) {
        err_sys("sigpending error");
    }

    /**
     * 检查某信号集中某个信号所对应位是否为1；
     * 如果是未决信号集且返回值为1，则说明此信号是未决的
     * 如果是阻塞信号集且返回值为1，则说明此信号是阻塞的
     */
    if (sigismember(&pendmask, SIGQUIT)) {
        printf("\nSIGQUIT pending\n");
    }

    /**
     * 从阻塞信号集中解除对SIGQUIT的阻塞（解除对SIGQUIT的阻塞）
     * 如果调用sigprocmask函数解除了对当前若干个未决信号的阻塞，则在该函数返回前，至少将其中之一递送给该进程
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        err_sys("SIGQUIT unblocked\n");
    }

    printf("SIGQUIT unblocked\n");

    /**
     *因为SIGQUIT信号处理函数sig_quit把信号的处理方式重置为系统默认动作，所以如果在这5秒钟之内
     *再产生SIGQUIT信号那么直接会导致进程终止+core dump
     */
    sleep(5);

    exit(0);
}

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) {  /* 改变SIGQUIT信号的处理方式为系统默认动作 */
        err_sys("can't reset SIGQUIT");
    }
}


/*实验：
[dendi875@localhost Chapter-10-Signals]$ ./sigprocmask
^\
SIGQUIT pending
caught SIGQUIT
SIGQUIT unblocked

[dendi875@localhost Chapter-10-Signals]$ ./sigprocmask
^\
SIGQUIT pending
caught SIGQUIT
SIGQUIT unblocked
^\退出 (core dumped)
[dendi875@localhost Chapter-10-Signals]$ ll /data/coredump/core.sigprocmask.10411
-rw------- 1 dendi875 dendi875 159744 4月   2 17:24 /data/coredump/core.sigprocmask.10411

验证[1-31]号信号是非可靠信号，非实时信号不支持信号排除，比如发送多次相同的信号，进程只能收到一次
[dend1i875@localhost Chapter-10-Signals]$ ./sigprocmask
^\^\^\^\^\^\^\^\^\^\^\
SIGQUIT pending
caught SIGQUIT
SIGQUIT unblocked
^\退出 (core dumped)

执行信号的处理动作称为信号递达（Delivery），信号从产生到递达之间的状态，称为信号未决（Pending）。进程可以选择阻塞（Block）某个信号。被阻塞的信号产生时将保持在未决状态，直到进程解除对此信号的阻塞，才执行递达的动作。注意，阻塞和忽略是不同的，只要信号被阻塞就不会递达，而忽略是在递达之后可选的一种处理动作。每个进程都有一个用来描述哪些信号递送到进程时将被阻塞的信号集，该信号集中的所有信号在递送到进程后都将被阻塞。
-----------------------------------------------------------------------------------------------------
pedding（未决信号集）：由内核来更改每个信号所对应位的值（改为1或0），用户只由获取信号所对应位的值不能更改。
如果某个信号在进程的阻塞集中，则该信号在未决集中所对应位的值为1，表示该信号不能被递达，不会被处理。使用sigpending来获取
block集（阻塞集、屏蔽集）：也称为屏蔽字是可以被用户检测或更改的，一个进程如果阻塞了某个信号，
则在block集中该信号所对应位的值为1。使用sigprocmask来获、更改
handler（信号处理函数集）：表示每个信号所对应的处理动作（系统默认、忽略、被捕捉），当信号不在未决集中时，将被调用
----------------------------------------------------------------------------------------------
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset); 用来检查或更改，或同时检测和更改进程的block集。

如果oldset是非空指针，则返回进程原来的阻塞信号集情况（被放入oldset中）。如果oldset是空指针则说明我们不关心原来阻塞信号集的情况。
set是非空指针时由参数how来控制如何修改当前进程的阻塞信号集：
SIG_BLOCK 在进程当前阻塞信号集中添加set指向信号集中的信号；信号屏蔽字是其当前信号屏蔽字和set指向信号集的并集，set包含了希望阻塞的信号，相当于：mask=mask|set
SIG_UNBLOCK  如果进程阻塞信号集中包含set指向信号集中的信号，则解除对该信号的阻塞；信号屏蔽字是其当前信号屏蔽字和set所指向信号集补集的交集，，set包含了希望解除阻塞的信号,相当于：mask=mask|~set
SIG_SETMASK  信号屏蔽字将被set指向的信号集的值代替；更新进程阻塞信号集为set指向的信号集，相当于mask=set

一个进程的信号屏蔽字规定了当前阻塞而不能递送给该进程的信号集，如果调用sigprocmask函数解除了对当前若干个未决信号的阻塞，则在该函数返回前，至少将其中一个信号递达。

sigpending(sigset_t *set)) 返回一个信号集，对于调用进程而言，其中的各信号是阻塞不能递送的，因而也一定是未决的。在set指向的信号集中返回结果。
sigsuspend(const sigset_t *mask))用于在接收到某个信号之前, 临时用mask替换进程的信号掩码, 并暂停进程执行，直到收到信号为止。
sigsuspend 返回后将恢复调用之前的信号掩码。信号处理函数完成后，进程将继续执行。该系统调用始终返回-1，并将errno设置为EINTR。

参考资料：
http://www.cnblogs.com/mickole/p/3191281.html
https://jin-yang.github.io/post/kernel-signal-introduce.html*/