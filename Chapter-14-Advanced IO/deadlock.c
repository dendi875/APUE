/**
 * 死锁实例
 *
 * 1、子进程对第0个字节加锁，父进程对第1个字节加锁。然后子进程试图对第1个字节加锁，
 * 父进程试图对第0个字节加锁。
 *
 * 2、该程序使用了信号实现的TELL_WAIT、TELL_PARENT、WAIT_PARENT、TELL_CHILD、WAIT_CHILD
 * 函数，保证父子进程之间进行同步。目的是为了使：
 * 子进程获取第一把锁之后等待父进程获取它第一把锁才能继续
 * 父进程获取第一把锁之后等待子进程获取它第一把锁才能继续
 *
 * 3、在检测到死锁时，内核必须选择一个进程出错返回。到底时选择父进程还是子进程不一定和实现
 * 细节相关
 */

#include "apue.h"
#include <signal.h>
#include <fcntl.h>

static volatile     sig_atomic_t    sigflag;
static              sigset_t        newmask, oldmask, zeromask;

/**
 * SIGUSR1和SIGUSR2信号处理函数
 */
static void sig_usr(int signo)
{
    sigflag = 1;
}

/**
 * 做一些初始化准备工作
 * 0、使用 sigaction 来注册SIGUSR1、SIGUSR2信号
 * 1、构造 newmask 使当前进程的阻塞信号集中的SIGUSR1、SIGUSR2信号变为阻塞状态
 * 2、构造 oldmask 用来保存进程原来的阻塞信号集状态，以便后续恢复
 * 3、构造 zeromask （空信号集）给sigsuspend用
 */
static void TELL_WAIT(void)
{
    struct sigaction    sa;

    sa.sa_handler = sig_usr;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) < 0) {
        err_sys("sigaction(SIGUSR1) error");
    }
    sa.sa_handler = sig_usr;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR2, &sa, NULL) < 0) {
        err_sys("sigaction(SIGUSR2) error");
    }

    sigemptyset(&zeromask);

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        err_sys("SIG_BLOCK error");
    }
}

/**
 * 子进程向父进程发送 SIGUSR2信号
 */
static void TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR2);
}

/**
 * 子进程在等待父进程向自己发送 SIGUSR1 信号期间休眠，如果 sigflags值为1说明父进程已经完成了
 * 自己的工作
 */
static void WAIT_PARENT(void)
{
    while (sigflag == 0) {
        sigsuspend(&zeromask);
    }

    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        err_sys("SIG_SETMASK error");
    }
}

/**
 * 父进程向子进程发送 SIGUSR1信号
 */
static void TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);
}

/**
 * 挂起父进程，等待子进程向自己发送信号
 */
static void WAIT_CHILD(void)
{
    while (sigflag == 0) {
        sigsuspend(&zeromask);
    }

    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        err_sys("SIG_SETMASK error");
    }
}

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type = type; /* F_RDLCK（读共享锁）, F_WRLCK（写独占锁）, F_UNLCK（解锁）*/
    lock.l_whence = whence; /* SEEK_SET，SEEK_CUR，SEEK_END */
    lock.l_start = offset;
    lock.l_len = len;

    return(fcntl(fd, cmd, &lock));
}

/**
 * 锁定一个字节
 */
static void lockabyte(const char *name, int fd, off_t offset)
{
    if (writew_lock(fd, offset, SEEK_SET, 1) < 0) {
        err_sys("%s：writew_lock error", name);
    }
    printf("%s：got the lock, byte %lld\n", name, (long long)offset);
}

int main(void)
{
    int     fd;
    pid_t   pid;

    /**
     * 创建一个文件并向文件中写入两个字节
     * create 等价于 使用open时指定flags 为　O_WRONLY | O_CREAT | O_TRUNC
     */
    if ((fd = creat("templock", FILE_MODE)) < 0) {
        err_sys("creat error");
    }
    if (write(fd, "ab", 2) != 2) {
        err_sys("write error");
    }

    TELL_WAIT();
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 子进程中 */
        lockabyte("child", fd, 0);
        TELL_PARENT(getppid()); /* 向父进程发送信号，告诉父进程我们已经处理完成了，它可以继续了*/
        WAIT_PARENT();
        lockabyte("child", fd, 1);
    } else {                    /* 父进程中 */
        lockabyte("parent", fd, 1);
        TELL_CHILD(pid);        /* 向子进程发送信号，告诉子进程我们已经处理完成了，它可以继续了*/
        WAIT_CHILD();
        lockabyte("parent", fd, 0);
    }

    exit(0);
}

/*
实验：
[root@localhost Chapter-14-Advanced IO]# ./deadlock
parent：got the lock, byte 1
child：got the lock, byte 0
child：writew_lock error: Resource deadlock avoided
parent：got the lock, byte 0

注意点：
1、TELL_WAIT是在fork之前调用，而TELL_WAIT会把进程的阻塞信号集中的 SIGUSR1和SIGUSR2对应置为1
（阻塞这两个信号）,而fork后子进程会继承父进程的阻塞信号集和信号处理方式。
2、子进程调用 TELL_PARENT之后因为父进程中的SIGUSR1信号和SIGUSR2信号处于阻塞未决状态，
所以父进程并不会马上接收到SIGUSR2信号，而是当父进程调用WAIT_CHILD时发现未决信号集中有信号没处理，
这时才将SIGUSR2信号递送给父进程，然后调用 sig_usr信号处理函数。同理，父进程调用TELL_CHILD的过程类似。
*/