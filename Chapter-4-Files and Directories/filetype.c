/**
 *打印文件类型
 */
#include "apue.h"

int main(int argc, char **argv)
{
    int i;
    struct stat buf;
    char *ptr;

    for (i = 1; i < argc; i++) {
        printf("%s：", argv[i]);
        if (lstat(argv[i], &buf) < 0) {
            err_ret("lstat error");
            continue;
        } else if (S_ISREG(buf.st_mode)) {
            ptr = "regular";
        } else if (S_ISDIR(buf.st_mode)) {
            ptr = "directory";
        } else if (S_ISCHR(buf.st_mode)) {
            ptr = "character special";
        } else if (S_ISBLK(buf.st_mode)) {
            ptr = "block special";
        } else if (S_ISFIFO(buf.st_mode)) {
            ptr = "fifo";
        } else if (S_ISLNK(buf.st_mode)) {
            ptr = "symbolic link";
        } else if (S_ISSOCK(buf.st_mode)) {
            ptr = "socket";
        } else {
            ptr = "** unknown mode **";
        }
        printf("%s\n", ptr);
    }

    exit(0);
}


/*实验：
[dendi875@localhost Chapter-4-Files and Directories]$ ./filetype /etc/passwd  /dev/ /dev/tty /dev/sr0 /dev/log /dev/cdrom

/etc/passwd：regular
/dev/：directory
/dev/tty：character special
/dev/sr0：block special
/dev/log：socket
/dev/cdrom：symbolic link*/