/**
 *创建一个具有空洞的文件
 */

#include "apue.h"
#include <fcntl.h>

 char buf1[] = "abcdefghij";
 char buf2[] = "ABCDEFGHIJ";

int main(int argc, char **argv)
{
    int fd;

    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        err_sys("creat error");
    }

    if (write(fd, buf1, 10) != 10) {
        err_sys("buf1 write error");
    }
    /* 现在offset = 10 */

    if (lseek(fd, 16384, SEEK_SET) == -1) {
        err_sys("lseek error");
    }q
    /* 现在offset = 16384 */

    if (write(fd, buf2, 10) != 10) {
        err_sys("buf2 write error");
    }
    /* 现在offset = 16394 */

    exit(0);
}


/**
 * creat()　创建一个新文件 等价于：
 * open(const char *pathname, O_WRONLY | O_CREAT | O_TRUNC, mode_t mode)
 * O_WRONLY 只写打开
 * O_CREAT  若此文件不存在则创建它，默认有了O_CREAT所以creat()必须指定第三个参数(文件权限)
 * O_TRUNC  如果此文件存在，而且为只写或读-写成功打开，则将其长度截断为0
 * 返回值：
 *
 *
 * write() 向打开的文件写数据。
 * 返回值：
 * 返回值通常与第三个参数相同，否则表示出错。
 * 注意：
 * 对于普通文件，写操作从文件的当前偏移量处开始。如果在打开文件时，指定了O_APPEND选项，
 * 则在每次写操作时，将文件偏移量设置在文件的当前结尾处。在一次成功写之后，该文件偏移
 * 量增加实际写的字节数。
 *
 *
 * offt_t lssek(int fd, off_t offset, int whence) 向一个打开的文件设置它的偏移量
 * 参数offset与参数whence有关
 * whence参数为SEEK_SET（0）：设置为距文件开始处offset个字节
 * whence参数为SEEK_CUR（1）：相对当前位置加offset
 * whence参数为SEEK_SET（2）：设置为文件长度加offset
 *
 * 返回值：失败返回-1，成功返回新文件偏移量。
 *
 * 空洞文件：文件偏移量可以大于文件的当前长度，在这种情况下，对该文件的下一次写将加长该文件，并在文件中构成一个空洞。位于文件中但没有写过的字节都被读为0。
 */