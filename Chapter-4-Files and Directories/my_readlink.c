/**
 * readlink函数使用实例；
 * 1.使用lstat获取文件信息，判断该文件是否是软链接
 * 2.使用readlink读取该软链接指定的字节数
 */

#include "apue.h"

#define BUFFER_SIZE 10

int main(int argc, char *argv[])
{
    char buf[BUFFER_SIZE];
    struct stat sb;

    if (argc != 2) {
        err_quit("usage：my_readlink <symbolic_file>");
    }
    if (lstat(argv[1], &sb) < 0) {
        err_sys("lstat error");
    }
    if (!S_ISLNK(sb.st_mode)) {
        err_quit("%s not a symbolic link", argv[1]);
    }
    if (readlink(argv[1], buf, BUFFER_SIZE) < 0) {
        err_sys("readlink error");
    }
    printf("buf = %s\n", buf);

    exit(0);
}

/**
 * 实验：
[root@localhost ctest]# echo '0123456789' > /test/ctest/testfile
[root@localhost ctest]# ln -s /test/ctest/testfile  ./testfile-so
[root@localhost ctest]# cat testfile-so
0123456789

[root@localhost ctest]# ./my_readlink testfile-so
buf = /test/ctes

注意：testfile-so软链接文件的block块中的内容存放的只是源文件的名字（/test/ctest/testfile）
20个字符。所以readlink读取的是软链接本身的block块中的内容，读取10个字符所以buf = /test/ctes
 */