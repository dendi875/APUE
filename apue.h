#ifndef _APUE_H
#define _APUE_H

#define _POSIX_C_SOURCE     200809L

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

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h> /* 字符串处理 */
#include <unistd.h> /* 多种必要的POSIX函数与常量 */
#include <signal.h>

#define MAXLINE             4096

#define FILE_MODE           (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define DIR_MODE            (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef void                Sigfunc(int);

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


#include "error.c"
#endif