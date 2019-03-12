/**
 * feof()和ferror()函数使用实例
 */

#include "apue.h"

static int find_int(const char *);

int main(int argc, char *argv[])
{
	FILE *fp;

	if (argc != 2) {
		err_quit("usage my_ferror <filename>");
	}
	printf("%d\n", find_int(argv[1]));

	exit(0);
}

/**
 * 搜索文件中以数字开始的行，并把这个数字返回
 *
 * 如果无法打开文件出错返回-1，
 * 如果发生读错误返回-2
 * 如果没有找到以数字开头的行(到达了文件末尾)返回-3，
 *
 * 假设文件中没有以负数开始的行
 */
static int find_int(const char *filename)
{
	FILE *fp;
	int n;

	if ((fp = fopen(filename, "r")) == NULL) {
		return -1;
	}

	while (fscanf(fp, "%d", &n) != 1) {
		if (ferror(fp)) {
			fclose(fp);
			return -2;
		}
		if (feof(fp)) {
			fclose(fp);
			return -3;
		}
		fscanf(fp, "%*[^\n]"); // 跳过全部字符至到遇到下一个换行符为至
	}
	fclose(fp);

	return n;
}


/*实验：
[root@localhost Chapter-5-Standard IO Library]# cat > /test/c_exercise/foo <<EOF
> aaaaaa
> bbbbbb
> 11afa
> ccccc
> EOF
[root@localhost Chapter-5-Standard IO Library]# ./my_ferror /test/c_exercise/foo
11

[root@localhost Chapter-5-Standard IO Library]# cat > /test/c_exercise/foo <<EOF
> aaaa
> bbbb
> ccc
> EOF
[root@localhost Chapter-5-Standard IO Library]# ./my_ferror /test/c_exercise/foo
-3

[root@localhost Chapter-5-Standard IO Library]# ./my_ferror /test/c_exercise/no_such_file
-1*/