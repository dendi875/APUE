#include "apue.h"
#include <pthread.h>
#include <string.h>

#define NAME_LEN    30

struct person {
    int index;
    char name[NAME_LEN + 1];
};

void *thr_fun1(void *p)
{
    printf("thread 1 tid = %lu\n", (unsigned long)pthread_self());

    ((struct person *)p)->index = 1;
    strcpy(((struct person *)p)->name, "dendi875");

    return((void *)0);
}

int main(void)
{
    pthread_t       tid1;
    int             err;
    struct person   p1;

    printf("main thread tid = %lu\n", (unsigned long) pthread_self());
    err = pthread_create(&tid1, NULL, thr_fun1, &p1);
    if (err != 0) {
        err_exit(err, "can't create thread 1");
    }

    sleep(1);

    printf("p1.index = %d\n", p1.index);
    printf("p1.name = %s\n", p1.name);

    exit(0);
}

/*实验：
[dendi875@localhost Chapter-11-Threads]$ ./my_pthread_create
main thread tid = 3078772416
thread 1 tid = 3078769520
p1.index = 1
p1.name = dendi875
*/