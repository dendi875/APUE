#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main(void)
{
	fputs(strsignal(SIGKILL), stdout);
	fputc('\n', stdout);

	exit(0);
}

/*实验：
[dendi875@192 Chapter-10-Signals]$ ./my_strsignal
Killed*/