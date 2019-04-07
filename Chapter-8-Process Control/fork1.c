#include "apue.h"

int globvar = 6;    /* 在虚拟内存中的初始化的数据 */
char buf[] = "a write to stdout\n";

int main(void)
{
    int var; /* 在虚拟内存中栈中的自动变量　*/
    pid_t pid;

    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) {
        err_sys("write error");
    }
    printf("befor fork\n");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {  /* 子进程中 */
        globvar++;
        var++;
    } else {
        sleep(2);    /* 父进程中 */
    }

    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    exit(0);
}


/*fork函数调用一次返回两次，在子进程中返回0，父进程返回子进程的PID。
为什么父进程中返回的是子进程的PID，因为一个进程可以创建多个子进程，但没有一个函数可以获取一个进程的
所有子进程ID（一般情况下要在父进程中保留所创建的子进程ID），子进程中返回0的理由是，子进程中总是可以
使用getppid获得父进程的进程ID。

父进程和子进程继续执行fork后的指令。子进程是父进程的副本。子进程获得父进程的数据段、堆和栈的副本。
子进程既然是一个进程那么它就有自己的虚拟地址空间，父子进程不共享这些储存空间，父子进程共享正文段。

fork它会使父进程打开的文件描述符都“复制”到子进程中。这里的“复制”指的是好像执行了dup函数，
从父进程中复制过来的那些文件描述符和父进程中的那些文件描述符指向同一文件表项。所以父进程中
的标准输出被重定向后，子进程的标准输出也被重定向。


实验一：
[dendi875@localhost Chapter-8-Process Control]$ ./fork1
a write to stdout
befor fork
pid = 13582, glob = 7, var = 89 // 子进程变量值改变了
pid = 13581, glob = 6, var = 88 // 父进程变量值没有改变

// printf默认是行缓冲的，当遇到换行符时进行I/O操作，然后父进程中虚拟地址空间数据段中的
// 内容就被冲洗了，fork后子进程虚拟地址空间的数据段中就没有printf("befor fork\n");

实验二：
[dendi875@localhost Chapter-8-Process Control]$ ./fork1 > temp.out
[dendi875@localhost Chapter-8-Process Control]$ cat temp.out
a write to stdout
befor fork                         // 这是子进程调用exit后输出的
pid = 13591, glob = 7, var = 89    // 这是子进程的PID
befor fork                         // 这是父进程调用exit后输出的
pid = 13590, glob = 6, var = 88    // 这是父进程的PID

// 标准输出（printf输出到stdout中）默认是行缓冲的（像实验一一样），当标准输出被重定向到文件
// 中时（ ./fork1 > temp.out），标准输出是全缓冲的，fork之前执行的那次printf并没有输出，
还在缓冲区中，当fork函数执行时在父进程虚拟地址空间的数据段中的printf("befor fork\n");还没有被冲洗掉（没输出），而是把父进程中的缓冲区数据复制了子进程中，然后父进程sleep(2)等
// 待子进程退出。当子进程调用exit退出程序时，因为exit正常终止程序时会默认执行标准I/O清理
// 动作，比如fclose（fclose在关闭文件之前会冲洗缓冲区的输出数据，缓冲区中的任何输出都将被
// 丢弃）。当父进程调用exit时也会执行标准I/O清理动作，所以befor fork会被输出两次

*/