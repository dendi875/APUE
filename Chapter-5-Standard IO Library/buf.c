/**
 * CentOS 6.3 FILE（_IO_FILE）结构体定义的位置： /usr/include/libio.h
 *
 * 对各个标准I/O流打印缓冲状态信息
 */
#include "apue.h"

void pr_stdio(const char *, FILE *);
int is_unbuffered(FILE *);
int is_linebuffered(FILE *);
int buffer_size(FILE *);

int main(void)
{
    FILE *fp;

    fputs("enter any character\n", stdout); /* 第一个I/O操作，为stdout流分配缓冲区 */
    if (getchar() == EOF) {                 /* 第一个I/O操作，为stdin流分配缓冲区 */
        err_sys("getchar error");
    }
    fputs("one line to standard error\n", stderr); /* 第一个I/O操作，为stderr流分配缓冲区 */

    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);

    if ((fp = fopen("/etc/passwd", "r")) == NULL) {
        err_sys("fopen error");
    }
    if (getc(fp) == EOF) {              /* 第一个I/O操作，为fp流分配缓冲区 */
        err_sys("getc error");
    }
    pr_stdio("/etc/passwd", fp);

    exit(0);
}

void pr_stdio(const char *name, FILE *fp)
{
    printf("stream = %s, ", name);
    if (is_unbuffered(fp)) {
        printf("unbuffered");
    } else if (is_linebuffered(fp)) {
        printf("line buffered");
    } else {
        printf("fully buffered");
    }
    printf(", buffer size = %d\n", buffer_size(fp));
}

#if defined(_IO_UNBUFFERED)

int is_unbuffered(FILE *fp)
{
    return(fp->_flags & _IO_UNBUFFERED);
}

int is_linebuffered(FILE *fp)
{
    return(fp->_flags & _IO_LINE_BUF);
}

int buffer_size(FILE *fp)
{
    return(fp->_IO_buf_end - fp->_IO_buf_base);
}

#elif defined(__SNBF)

int is_unbuffered(FILE *fp)
{
    return(fp->_flags & __SNBF);
}

int is_linebuffered(FILE *fp)
{
    return(fp->_flags & __SLBF);
}

int buffer_size(FILE *fp)
{
    return(fp->_bf.size);
}

#elif defined(_IONBF)

#ifdef _LP64
#define _flag __pad[4]
#define _ptr __pad[1]
#define _base __pad[2]
#endif /* _LP64 */

int is_unbuffered(FILE *fp)
{
    return(fp->_flag & _IONBF);
}

int is_linebuffered(FILE *fp)
{
    return(fp->_flag & _IOLBF);
}

int buffer_size(FILE *fp)
{
#ifdef _LP64
    return(fp->_base - fp->_ptr);
#else
    return(BUFSIZ); /* just a guess */
#endif
}

#else

#error unknown stdio implementation!

#endif  /* _IO_UNBUFFERED */



/*实验：
三个标准I/O流（stdin，stdout，stderr）都与终端相关联
[root@localhost Chapter-5-Standard IO Library]# ./buf
enter any character

one line to standard error
stream = stdin, line buffered, buffer size = 1024
stream = stdout, line buffered, buffer size = 1024
stream = stderr, unbuffered, buffer size = 1
stream = /etc/passwd, fully buffered, buffer size = 4096

使三个流重定向，使它们与普通文件相关联（注意：2> std.err，2与>之间没有空格）
[root@localhost Chapter-5-Standard IO Library]# ./buf < /etc/passwd > std.out 2> std.err

[root@localhost Chapter-5-Standard IO Library]# cat std.err
one line to standard error
[root@localhost Chapter-5-Standard IO Library]# cat std.out
enter any character
stream = stdin, fully buffered, buffer size = 4096
stream = stdout, fully buffered, buffer size = 1024
stream = stderr, unbuffered, buffer size = 1
stream = /etc/passwd, fully buffered, buffer size = 4096
*/