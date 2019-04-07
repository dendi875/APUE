#include "apue.h"
#include <signal.h>

int main(void)
{
	psignal(SIGTSTP, "SIGTSTP");
	psignal(SIGSTOP, "SIGSTOP");
	psignal(SIGTTIN, "SIGTTIN");
	psignal(SIGTTOU, "SIGTTOU");
	psignal(SIGCONT, "SIGCONT");

	exit(0);
}

/*实验：
[dendi875@192 Chapter-10-Signals]$ ./my_psignal
SIGTSTP: Stopped
SIGSTOP: Stopped (signal)
SIGTTIN: Stopped (tty input)
SIGTTOU: Stopped (tty output)
SIGCONT: Continued*/