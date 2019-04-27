/**
 * 调用getlogin函数查看守护进程是否有登录名（登录系统时用的用户名）
 */

#include "apue.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <syslog.h>

void daemonize(const char *cmd)
{
	int 				i, fd0, fd1, fd2;
	pid_t				pid;
	struct rlimit 		rl;
	struct sigaction 	sa;

	/**
	 * 重新设置文件权限掩码值
	 */
	umask(0);

	/**
	 * 获取进程能够生成的最大文件描述符数量
	 */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
		err_quit("%s：can't get file limit", cmd);
	}

	/**
	 * fork一个子进程并创建一个新会话，使子进程成为会话首进程，成为进程组组长，脱离控制终端
	 */
	if ((pid = fork()) < 0) {
		err_sys("%s：fork error", cmd);
	} else if (pid > 0) {
		exit(0);
	}
	setsid();

	/**
	 * 忽略SIGHUP信号，并再次fork
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0) {
		err_sys("%s：can't ignore SIGHUP", cmd);
	}
	if ((pid = fork()) < 0) {
		err_quit("%s：can't fork", cmd);
	} else if (pid > 0) {
		exit(0);
	}

	/**
	 * 改变当前工作目录为根目录
	 */
	if (chdir("/") < 0) {
		err_quit("%s：can't change directory to /", cmd);
	}

	/**
	 * 关闭所有的文件描述符
	 */
	if (rl.rlim_max == RLIM_INFINITY) {
		rl.rlim_max = 1024;
	}
	for (i = 0; i < rl.rlim_max; i++) {
		close(i);
	}

	/**
	 * 使0、1、2文件描述符指向 /dev/null
	 */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/**
	 * 如果出错使用syslog记录日志
	 */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}

int main(void)
{
	FILE *fp;
	char *p;

	daemonize("getlog");
	p = getlogin();
	fp = fopen("/tmp/getlog.out", "w");
	if (fp != NULL) {
		if (p == NULL) {
			fprintf(fp, "no login name\n");
		} else {
			fprintf(fp, "login name：%s\n", p);
		}
	}

	exit(0);
}

/*
实验：
[dendi875@192 Chapter-13-Daemon Processes]$ cat /tmp/getlog.out
login name：dendi875
*/