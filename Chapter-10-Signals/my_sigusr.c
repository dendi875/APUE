int main(void)
{
    while (1) {
        ;
    }

    return 0;
}

/*[dendi875@localhost Chapter-10-Signals]$ ./my_sigusr &
[1] 26110
[dendi875@localhost Chapter-10-Signals]$ jobs -l
[1]+ 26110 Running                 ./my_sigusr &
[dendi875@localhost Chapter-10-Signals]$ ps ajxf | grep my_sigusr
17600 26110 26110 17600 pts/0    26142 R      504   0:20              \_ ./my_sigusr
17600 26143 26142 17600 pts/0    26142 S+     504   0:00              \_ grep --color=auto my_sigusr

man查看SIGUSR1、SIGUSR2这两个用户自定义信号系统默认处理方式是终止进程
[dendi875@localhost Chapter-10-Signals]$ man signal

给进程发送SIGUSR1信号
[dendi875@localhost Chapter-10-Signals]$ kill -SIGUSR1 26110

再次查看进程已经终止了
[dendi875@localhost Chapter-10-Signals]$ jobs -l

[dendi875@localhost Chapter-10-Signals]$ ps ajxf | grep my_sigusr
17600 26153 26152 17600 pts/0    26152 S+     504   0:00              \_ grep --color=auto my_sigusr
[1]+  用户定义信号 1    ./my_sigusr*/