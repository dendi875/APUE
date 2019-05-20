/**
 * 使用read/write阻塞I/O，来翻译文件（翻译算法为ROT-13）
 *
 * ROT-13算法：将文本中的英文字符（a~z和A-Z）分别向右偏移13个字母位移，其它字符保持不变
 */
#include "apue.h"
#include <fcntl.h>
#include <ctype.h>

#define  BSZ  100

unsigned char buf[BSZ];

unsigned char translate(unsigned char c)
{
    if (isalpha(c)) {
        if (c >= 'n') {
            c -= 13;
        } else if (c >= 'a') {
            c += 13;
        } else if (c >= 'N') {
            c -= 13;
        } else {
            c += 13;
        }
    }
    return(c);
}

int main(int argc, char *argv[])
{
    int ifd, ofd, n, i, nw;

    if (argc != 3) {
        err_quit("usage：./rot13 infile outfile");
    }
    if ((ifd = open(argv[1], O_RDONLY)) < 0) {
        err_sys("can't open %s", argv[1]);
    }
    if ((ofd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
        err_sys("can't create %s", argv[2]);
    }

    while ((n = read(ifd, buf, BSZ)) > 0) {
        for (i = 0; i < n; i++) {
            buf[i] = translate(buf[i]);
        }
        if ((nw = write(ofd, buf, n)) != n) {
            if (nw < 0) {
                err_sys("write failed");
            } else {
                err_quit("short write (%d/%d)", nw, n);
            }
        }
    }

    fsync(ofd);

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-14-Advanced IO]$ ./rot13 temp.file outfile
*/