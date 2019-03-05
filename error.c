#include "apue.h"
#include <errno.h>
#include <stdarg.h>

static void err_doit(int, int, const char *, va_list);

/**
 * 系统调用相关的非致命错误
 */
void err_ret(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
}

/**
 * 系统调用相关的致命错误
 * 打印一条消息，正常终止
 */
void err_sys(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    exit(1);
}

/**
 * 系统调用无关的非致命错误
 */
void err_msg(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
}

/**
 * 系统调用无关的致命错误
 * 打印一条消息，正常终止
 */
void err_quit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
    exit(1);
}

/**
 * 系统调用相关的致使错误。
 * 打印一条消息，异常终止
 */
void err_dump(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    abort();
    exit(1);
}

static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char    buf[MAXLINE];

    /**
     * 把可变参数，ap，按fmt格式化，写入buf字符数组中，最多写MAXLINE个字符
     */
    vsnprintf(buf, MAXLINE, fmt, ap);
    if (errnoflag) {
        snprintf(buf + strlen(buf), MAXLINE - strlen(buf), ": %s", strerror(error));
    }
    strcat(buf, "\n");
    fflush(stdout);
    fputs(buf, stderr);
    fflush(NULL);
}