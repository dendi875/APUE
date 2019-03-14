/**
 * 复制一个文件，例如把文件f1.c复制给文件f2.c，可以使用命令 my_fcopy f1.c f2.c
 */

#include "apue.h"

int main(int argc, char **argv)
{
    FILE *source_fp, *dest_fp;
    int c;

    if (argc != 3) {
        err_quit("usage：my_fcopy source dest");
    }
    if ((source_fp = fopen(*++argv, "rb")) == NULL) {
        err_sys("fopen error for %s", *++argv);
    }
    if ((dest_fp = fopen(*++argv, "wb")) == NULL) {
        err_sys("fopen error for %s", *++argv);
    }
    while ((c = fgetc(source_fp)) != EOF) {
        if (fputc(c, dest_fp) == EOF) {
            err_sys("fputc error");
        }
    }

    if (ferror(source_fp)) {
        err_sys("fgetc error");
    }

    fclose(source_fp);
    fclose(dest_fp);

    exit(0);
}


/*实验：
创建源文件（文本文件）
[root@localhost Chapter-5-Standard IO Library]# cat > /test/ctest/bar <<EOF
> 123
> abc
> EOF

运行程序
[root@localhost Chapter-5-Standard IO Library]# ./my_fcopy /test/ctest/bar /test/ctest/foo

查看目录文件的内容
[root@localhost Chapter-5-Standard IO Library]# cat /test/ctest/foo
123
abc
*/