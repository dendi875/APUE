#include "apue.h"

int main(int argc, char **argv)
{
    printf("hello world from process ID %ld\n", (long int) getpid());

    exit(0);
}
