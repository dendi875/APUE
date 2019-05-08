/**
 * 测试一个系统是否支持强制性建议锁
 *
 * 1、创建一个文件，开启文件的强制性建议锁特性（如果是命令行操作就是 chmod 2664 filename）
 * 2、程序分出两个进程，父进程对整个文件设置一把写锁，子进程先把该文件的描述符设置为非阻塞的，然后
 * 试图对该整个文件设置一把读锁，我们期望会出错返回。接着，子进程将文件读、写指针位置调整到文件开始
 * 处，并试图读文件。如果系统是支持强制性建议锁功能，则read应该返回EAGAIN（因为描述符是非阻塞的），
 * 否则 read返回所读的数据
 */

#include "apue.h"
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>

static volatile sig_atomic_t    sigflag;
static sigset_t                 newmask, oldmask, zeromask;

/**
 * SIGUSR1和SIGUSR2信号处理函数
 */
void sig_usr(int signo)
{
    sigflag = 1;
}

/**
 * 做一些初始化准备工作
 *
 * 1、注册 SIGUSR1、SIGUSR2　信号处理函数，当接收到这两个信号时修改全局、易失、原子类型的变量
 * 2、构造 newmask　来阻塞当前进程的 SIGUSR1、SIGUSR2　信号，构造 oldmask 来保存原有的阻塞信号集
 * 为什么要阻塞这两个信号，是为了防止在调用TELL_PARENT、和TELL_CHILD之前有其它的进程向本进程
 * 发送这两个信号修改了 sigflag 的值，
 * 3、构造 zeromask（空信号集） 来给 sigsuspen 函数使用，用空信号集在挂起进程期间临时替换进程的
 * 阻塞信号集，以便我们能发送信号
 */
void TELL_WAIT(void)
{
    struct sigaction sa;

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

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        err_sys("SIG_BLOCK error");
    }

    sigemptyset(&zeromask);
}

/**
 * 子进程向父进程发送 SIGUSR2 信号
 */
void TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR2);
}

void WAIT_PARENT(void)
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
 * 父进程向子进程发送 SIGUSR1 信号
 */
void TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);
}

void WAIT_CHILD(void)
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

    lock.l_type = type;
    lock.l_whence = whence;
    lock.l_start = offset;
    lock.l_len = len;

    return(fcntl(fd, cmd, &lock));
}

void set_fl(int fd, int flags)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("fcntl F_GETFL error");
    }

    val |= flags; /* turn on flags */

    if ((fcntl(fd, F_SETFL, val)) < 0) {
        err_sys("fcntl F_SETFL error");
    }
}

int main(int argc, char *argv[])
{
    int             fd;
    struct stat     sb;
    pid_t           pid;
    char            buf[5];

    if (argc != 2) {
        fprintf(stderr, "usage：%s filename\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC,  FILE_MODE)) < 0) {
        err_sys("open error");
    }
    if (write(fd, "abcdef", 6) != 6) {
        err_sys("write error");
    }

    /**
     * 关闭所属组的执行位并给文件加上SGID权限
     */
    if (fstat(fd, &sb) < 0) {
        err_sys("fstat error");
    }
    if (fchmod(fd, (sb.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
        err_sys("fchmod error");
    }

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {   /* 父进程 */
        /* 对整个文件加一把写独占锁 */
        if (write_lock(fd, 0, SEEK_SET, 0) < 0) {
            err_sys("write_lock error");
        }

        /* 向子进程发送信号，告诉子进程可以继续执行了 */
        TELL_CHILD(pid);

        if (waitpid(pid, NULL, 0) < 0) {
            err_sys("waitpid error");
        }
    } else {                /* 子进程 */
        /* 等待父进程向我们发送信号后，才能继续 */
        WAIT_PARENT();

        /* 设置文件描述符为非阻塞 */
        set_fl(fd, O_NONBLOCK);

        if (read_lock(fd, 0, SEEK_SET, 0) != -1) {
            err_sys("child：read_lock successed");
        }
        printf("read_lock of already-locked region returns %d\n", errno);

        /* 把文件读写指针位置设置到文件起始处 */
        if (lseek(fd, 0, SEEK_SET) == -1) {
            err_sys("lseek error");
        }

        /**
         * 在另一个进程持有文件（且文件是开启了强制性建议锁特性）独占写锁情况下，
         * 再去read文件内容，如果系统支持强制性建议锁因为文件描述符是非阻塞的，那么应当
         * 返回 EAGAIN或，否则系统不运行强制性建议锁那么read则会成功读取到内容
         */
        if (read(fd, buf, 2) < 0) {
            err_ret("read failed （mandatory locking works）");
        } else {
            printf("read OK（no mandatory locking）, buf = %2.2s\n", buf);
        }
    }

    exit(0);
}

/*
实验：
[dendi875@localhost ctest]$ ./mandatory temp.lock
read_lock of already-locked region returns 11
read OK（no mandatory locking）, buf = ab
[dendi875@localhost ctest]$ ll temp.lock
-rw-r-Sr-- 1 dendi875 dendi875 6 4月  29 14:45 temp.lock
*/