/**
 *使用信号机制来实现多个进程之间的同步。　
 *
 *使用SIGUSR1、SIGUSR2这两个用户自定义信号：
 *SIGUSR1由父进程发送给子进程，SIGUSR2由子进程发送给父进程。
 */

#include "apue.h"
#include <signal.h>
#include <unistd.h>

/**
 * 全局变量sigflag默认值为0，在SIGUSR1和SIGUSR2信号处理函数中将改变这个值为1
 */
static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int);
static void TELL_WAIT(void);
static void TELL_PARENT(pid_t);
static void WAIT_PARENT(void);
static void TELL_CHILD(pid_t);
static void WAIT_CHILD(void);
static void charatatime(char *);

int main(void)
{
	pid_t	pid;

	TELL_WAIT();

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {		/* 在子进程中 */
		charatatime("output from child\n");
		/**
		 * 子进程处理完成了，向父进程发送信号，告诉父进程我们执行完了你可以继续执行了
		 */
		TELL_PARENT(getppid());
	} else {					/* 在父进程中 */
		/**
		 * 父进程休眠等待子进程发送信号后才能继续执行
		 */
		WAIT_CHILD();
		charatatime("output from parent\n");
	}

	exit(0);
}

/**
 * SIGUSR1和SIGUSR2信号处理函数
 */
static void sig_usr(int signo)
{
	sigflag = 1;
}

/**
 * 做一些初始化准备工作
 *
 * 1、注册 SIGUSR1、SIGUSR2　信号处理函数，当接收到这两个信号时修改全局、易失、原子类型的变量
 * 2、构造 newmask　来阻塞当前进程的 SIGUSR1、SIGUSR2　信号，构造 oldmask 来保存原有的阻塞信号集
 * 为什么要阻塞这两个信号，是为了防止在调用TELL_PARENT、和TELL_CHILD之前有其它的进程向本进程
 * 发送这两个信号修改了 sigflag 的值，防止在调用 TELL_WAIT 之后和调用 WAIT_PARENT 之前有其它进程向
 * 自己发送 SIGUSR1或SIGUSR2 信号
 * 3、构造 zeromask（空信号集） 来给 sigsuspend 函数使用，用空信号集在挂起进程期间临时替换进程的
 * 阻塞信号集，以便我们能使用 kill 函数发送信号。 man sigsuspend
 */
static void TELL_WAIT(void)
{
	auto struct sigaction usr1_act, usr2_act;

	usr1_act.sa_handler = sig_usr;
	sigemptyset(&usr1_act.sa_mask);
	usr1_act.sa_flags = 0;

	usr2_act.sa_handler = sig_usr;
	sigemptyset(&usr2_act.sa_mask);
	usr2_act.sa_flags = 0;

	if (sigaction(SIGUSR1, &usr1_act, NULL) < 0) {
		err_sys("sigaction(SIGUSR1) error");
	}
	if (sigaction(SIGUSR2, &usr2_act, NULL) < 0) {
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
 * 子进程向父进程发送SIGUSR2信号
 */
static void TELL_PARENT(pid_t pid)
{
	kill(pid, SIGUSR2);
}

/**
 * 子进程在等待父进程向自己发送SIGUSR1信号期间休眠，如果父进程发送了SIGUSR1信号那么全局变量的
 * 值为1
 */
static void WAIT_PARENT(void)
{
	/**
	 * 为什么用 while 用 if 不行吗？
	 */
	while (sigflag == 0) {
		sigsuspend(&zeromask);
	}

	/* sigflag值已经改变，为了使父子进程下次能同步成功，要恢复到原来的值 */
	sigflag = 0;

	/**
	 * 为了能使父子进程之间同步多次（在不同地方调用多次TELL_WAIT，TEll_*和WAIT_*），
	 * 这里要把SIGUSR1和SIGUSR2从当前进程阻塞信号集中解除了
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		err_sys("SIG_SETMASK error");
	}
}

/**
 * 父进程向子进程发送SIGUSR1信号
 */
static void TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR1);
}

/**
 * 在子进程向父进程发送SIGUSR2信号期间父进程要休眠
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

static void charatatime(char *str)
{
	char *ptr;
	int c;

	/**
	 * 关闭标准输出的默认行缓冲，这样每次调用fpuc都会调用底层系统调用函数write
	 */
	setbuf(stdout, NULL);
	for (ptr = str; (c = *ptr) != '\0'; ptr++) {
		fputc(c, stdout);
	}
}



/*
实验：
[dendi875@192 Chapter-10-Signals]$ ./tellwait
output from child
output from parent

执行多次
[dendi875@192 Chapter-10-Signals]$ ./tellwait; ./tellwait; ./tellwait
output from child
output from parent
output from child
output from parent
output from child
output from parent

修改main函数代码如下，使父进程先执行使父进程先执行
if ((pid = fork()) < 0) {
	err_sys("fork error");
} else if (pid == 0) {
	WAIT_PARENT();
	charatatime("output from child\n");
} else {
	charatatime("output from parent\n");
	TELL_CHILD(pid);
}

运行程序
[dendi875@192 Chapter-10-Signals]$ ./tellwait
output from parent
[dendi875@192 Chapter-10-Signals]$ output from child

执行多次
[dendi875@192 Chapter-10-Signals]$ ./tellwait; ./tellwait; ./tellwait
output from parent
output from child
output from parent
output from parent
[dendi875@192 Chapter-10-Signals]$ output from child
output from child

出现这种情况的原因是：我们先让父进程输出，然后父进程完成后发送信号让子进程输出，但子进程刚要输出时
父进程可能已经终止了。是父进程先终止还是在子进程先执行输出（这是一个竞争条件）。
如果父进程先终止后，子进程再输出那么shell就认为它的子进程（父进程）已经终止了，那么shell就会开始
执行下一个./tellwait程序。这样就会干扰子进程的输出。为了避免这种情况发生，我们要保存子进程输出后
父进程才能终止。修改为下面的代码：
if ((pid = fork()) < 0) {
	err_sys("fork error");
} else if (pid == 0) {
	WAIT_PARENT();
	charatatime("output from child\n");
	TELL_PARENT(getppid());
} else {
	charatatime("output from parent\n");
	TELL_CHILD(pid);
	WAIT_CHILD();
}

运行程序：
[dendi875@192 Chapter-10-Signals]$ ./tellwait; ./tellwait; ./tellwait
output from parent
output from child
output from parent
output from child
output from parent
output from child

*/