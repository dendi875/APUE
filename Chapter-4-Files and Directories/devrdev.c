#include "apue.h"
#include <sys/sysmacros.h>

int main(int argc, char *argv[])
{
	int i;
	struct stat statbuf;
	char *s;

	for (i = 1; i < argc; i++) {
		printf("%s：", argv[i]);
		if (stat(argv[i], &statbuf) < 0) {
			err_ret("stat error");
			continue;
		}
		printf("dev = %d/%d", major(statbuf.st_dev), minor(statbuf.st_dev));
		if (S_ISCHR(statbuf.st_mode) || S_ISBLK(statbuf.st_mode)) {
			s = (S_ISCHR(statbuf.st_mode)) ? "character" : "block" ;
			printf(" (%s) rdev = %d/%d", s, major(statbuf.st_rdev), minor(statbuf.st_rdev));
		}
		printf("\n");
	}
	exit(0);
}


/*实验：
运行程序
[root@localhost Chapter-4-Files and Directories]# ./devrdev / /home/zq /boot /dev/tty[01] /dev/sda[12345]
/：dev = 8/5
/home/zq：dev = 8/2
/boot：dev = 8/1
/dev/tty0：dev = 0/5 (character) rdev = 4/0
/dev/tty1：dev = 0/5 (character) rdev = 4/1
/dev/sda1：dev = 0/5 (block) rdev = 8/1
/dev/sda2：dev = 0/5 (block) rdev = 8/2
/dev/sda3：dev = 0/5 (block) rdev = 8/3
/dev/sda4：dev = 0/5 (block) rdev = 8/4
/dev/sda5：dev = 0/5 (block) rdev = 8/5

查看所有硬盘的分区信息
[root@localhost Chapter-4-Files and Directories]# fdisk -l

Disk /dev/sda: 21.5 GB, 21474836480 bytes
255 heads, 63 sectors/track, 2610 cylinders
Units = cylinders of 16065 * 512 = 8225280 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x00062b93

   Device Boot      Start         End      Blocks   Id  System
/dev/sda1   *           1          26      204800   83  Linux
Partition 1 does not end on cylinder boundary.
/dev/sda2              26         281     2048000   83  Linux
Partition 2 does not end on cylinder boundary.
/dev/sda3             281         409     1024000   82  Linux swap / Solaris
Partition 3 does not end on cylinder boundary.
/dev/sda4             409        2611    17693696    5  Extended
/dev/sda5             409        2611    17692672   83  Linux


查看所有文件系统挂载情况
[root@localhost Chapter-4-Files and Directories]# mount
/dev/sda5 on / type ext4 (rw)
proc on /proc type proc (rw)
sysfs on /sys type sysfs (rw)
devpts on /dev/pts type devpts (rw,gid=5,mode=620)
tmpfs on /dev/shm type tmpfs (rw,rootcontext="system_u:object_r:tmpfs_t:s0")
/dev/sda1 on /boot type ext4 (rw)
/dev/sda2 on /home type ext4 (rw)
none on /proc/sys/fs/binfmt_misc type binfmt_misc (rw)
.host:/ on /mnt/hgfs type vmhgfs (rw,ttl=1)


改系统中只有一个硬盘sda，字母sd代表硬盘接口类型是SATA类型接口的硬盘，字母a代表第一块SATA硬盘的第一个分区，
1.一块硬盘上主分区加上扩展分区最多有4个
2.扩展分区最多只能有一个
3.逻辑分区包含在扩展分区里面
4.能够被格式化作为数据访问的分区只有主分区与逻辑分区，扩展分区不能被格式化
5./dev/sda1，/dev/sda2 /dev/sda3 /dev/sda4 这sda后面的1,2,3,4个号码是保留给主分区或扩展分区使用，所以逻辑分区
的设备号码从数字5开始
*/