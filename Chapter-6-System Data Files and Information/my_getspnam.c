/**
 * getspnam函数使用实例
 */

#include "apue.h"
#include <pwd.h>
#include <shadow.h>

int main(void)
{
	struct passwd *ptr;
	struct spwd *sptr;

	if ((ptr = getpwuid(getuid())) == NULL) {
		err_sys("getpwuid error");
	}

	printf("username = %s\n", ptr->pw_name);
	if ((sptr = getspnam(ptr->pw_name)) == NULL) {
		err_sys("getspnam error");
	}
	printf("encrypted passwd = %s\n", sptr->sp_pwdp);

	endpwent();
	endspent();

	exit(0);
}


/*实验：
[zq@localhost ctest]$ ./my_getspnam
username = zq
getspnam error: Permission denied


[root@localhost ctest]# chown root my_getspnam

[root@localhost ctest]# chmod 4755 my_getspnam

[root@localhost ctest]# ll my_getspnam
-rwsr-xr-x. 1 root zq 11077 3月  14 23:16 my_getspnam

[zq@localhost ctest]$ ./my_getspnam
username = zq
encrypted passwd = $6$BQRzU7jh$PUxPsykBeMMj.Vki/7YVfgYLFitY1ajlLTEjXvZnyEDVy5leAl3vdcvf7cF.yAALDCtY7LhARWG5cPBClpL0K1
*/
