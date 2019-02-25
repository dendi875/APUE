#include "apue.h"

int main(int argc, char **argv)
{
	off_t currpos;

	if ((currpos = lseek(STDIN_FILENO, 0, SEEK_CUR)) == -1) {
		printf("cannot seek\n");
	} else {
		printf("seek OK\n");
	}

	exit(0);
}
