/**
 * times函数实例，获取自己当前进程以及已终止子进程的，墙上时钟时间、用户CPU时间和系统CPU时间
 *
 * time(1)命令
 */

#include "apue.h"
#include <sys/times.h>

static void pr_times(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);
void pr_exit(int);

int main(int argc, char *argv[])
{
    int i;

    setbuf(stdout, NULL);   /* 关闭标准输出默认的行缓冲 */
    for (i = 1; i < argc; i++) {
        do_cmd(argv[i]);    /* 每个命令行参数执行一次 */
    }
    exit(0);
}

static void pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
    static long clktck = 0;

    if (clktck == 0) {  /* 获取每秒钟的滴答数 */
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0) {
            err_sys("sysconf error");
        }
    }

    printf("  real：%7.2f\n", real / (double) clktck);
    printf("  user：%7.2f\n", (tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
    printf("  sys：%7.2f\n", (tmsend->tms_stime - tmsstart->tms_stime) / (double) clktck);
    printf("  child user：%7.2f\n", (tmsend->tms_cutime - tmsstart->tms_cutime) / (double) clktck);
    printf("  child sys：%7.2f\n", (tmsend->tms_cstime - tmsstart->tms_cstime) / (double) clktck);
}

static void do_cmd(char *cmd)
{
    struct tms  tmsstart, tmsend;
    clock_t     start, end;
    int         status;

    printf("\ncomand：%s\n", cmd);

    if ((start = times(&tmsstart)) == -1) { /* 开始值 */
        err_sys("times error");
    }

    if ((status = system(cmd)) < 0) {       /* 执行命令 */
        err_sys("system() error");
    }

    if ((end = times(&tmsend)) == -1) {     /* 结束值 */
        err_sys("times error");
    }
    pr_times(end - start, &tmsstart, &tmsend);
    pr_exit(status);
}


void pr_exit(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termination, signal number = %d%s\n",
            WTERMSIG(status),
#ifdef WCOREDUMP
            WCOREDUMP(status) ? "（core file generated）" : "");
#else
            "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stoped, signal number = %dn", WSTOPSIG(status));
    }
}

/*
实验：
[dendi875@localhost Chapter-8-Process Control]$ ./times1 "sleep 5" "date" "man bash > /dev/null"

comand：sleep 5
  real：   5.03
  user：   0.00
  sys：   0.00
  child user：   0.00
  child sys：   0.02
normal termination, exit status = 0

comand：date
2019年 03月 26日 星期二 20:29:13 CST
  real：   0.00
  user：   0.00
  sys：   0.00
  child user：   0.00
  child sys：   0.00
normal termination, exit status = 0

comand：man bash > /dev/null
  real：   0.39
  user：   0.00
  sys：   0.00
  child user：   0.30
  child sys：   0.02
normal termination, exit status = 0
*/