#ifndef _APUE_H
#define _APUE_H

/**
 * 定义功能测试宏 _POSIX_C_SOURCE
 */
#define _POSIX_C_SOURCE     200809L

/**
 * 定义功能测试宏 _XOPEN_SOURCE
 */
#if defined(SOLARIS)
#define _XOPEN_SOURCE       600
#else
#define _XOPEN_SOURCE       700
#endif


#include <sys/types.h>  /* 系统数据类型 */
#include <sys/stat.h>   /* 文件信息 */
#include <sys/termios.h> /* 允许串口界面 */

#if defined(MACOS) || !defined(TIOCGWINSZ)
#include <sys/ioctl.h>
#endif

#include <stdio.h>  /* 标准I/0 */
#include <stdlib.h> /* 通过的实用工具 */
#include <stddef.h> /* 常用定义 */
#include <string.h> /* 字符串处理 */
#include <unistd.h> /* 多种必要的POSIX函数与常量 */
#include <signal.h> /* 信号处理 */

#define MAXLINE             4096

#define FILE_MODE           (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)     // 644
#define DIR_MODE            (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)   // 755

typedef void Sigfunc(int);

#define min(a, b)           ((a) < (b) ? (a) : (b))
#define max(a, b)           ((a) > (b) ? (a) : (b))

/*********************************************************************
*   输出至stderr的出错函数                                           *
*********************************************************************/

/**
 * 系统调用相关的非致命错误
 */
void    err_ret(const char *, ...);

/**
 * 系统调用相关的致命错误
 */
void    err_sys(const char *, ...) __attribute__((noreturn));


/**
 * 系统调用无关的非致命错误
 */
void    err_msg(const char *, ...);

/**
 * 系统调用无关的致命错误
 */
void    err_quit(const char *, ...) __attribute__((noreturn));


/**
 * 系统调用相关的致使错误。
 * 打印一条消息，异常终止
 */
void    err_dump(const char *, ...) __attribute__((noreturn));

/**
 * 系统调用无关的非致使错误。
 * 错误代码是通过参数传递进来的。
 * 打印一条消息，然后继续运行
 * cont（继续）
 */
void   err_cont(int, const char *, ...);

/**
 * 系统调用无关的致命错误。
 * 错误代码是通过参数传递进来的。
 * 打印一条消息，然后正常终止
 */
void    err_exit(int, const char *, ...) __attribute__((noreturn));


#include "error.c"
#endif