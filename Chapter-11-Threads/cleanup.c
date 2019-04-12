/**
 * 线程清理处理程序实例
 */

#include "apue.h"
#include <pthread.h>

void cleanup(void *arg)
{
	printf("cleanup：%s\n", (char *)arg);
}

void *thr_fun1(void *arg)
{
	printf("thread 1 start\n");
	pthread_cleanup_push(cleanup, "thread 1 first handler");
	pthread_cleanup_push(cleanup, "thread 1 second handler");
	printf("thread 1 push complete\n");
	if (arg) {
		return((void *)1);
	}
	/* 不能省略，是用来匹配pthread_cleanup_push的字符，如果省略会编译出错 */
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	return((void *)1);
}

void *thr_fun2(void *arg)
{
	printf("thread 2 start\n");
	pthread_cleanup_push(cleanup, "thread 2 first handler");
	pthread_cleanup_push(cleanup, "thread 2 second handler");
	printf("thread 2 push complete\n");
	if (arg) {
		pthread_exit((void *)2);
	}
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_exit((void *)2);
}

int main(void)
{
	pthread_t 	tid1, tid2;
	int 		err;
	void 		*retval;

	/* 创建线程tid1 */
	err = pthread_create(&tid1, NULL, thr_fun1, (void *)1);
	if (err != 0) {
		err_exit(err, "can't create thread 1");
	}

	/* 创建线程tid2 */
	err = pthread_create(&tid2, NULL, thr_fun2, (void *)1);
	if (err != 0) {
		err_exit(err, "can't create thread 2");
	}

	/* 阻塞等待回收tid1线程 */
	err = pthread_join(tid1, &retval);
	if (err != 0) {
		err_exit(err, "can't join with thread 1");
	}
	printf("thread 1 exit code %ld\n", (long)retval);

	/* 阻塞等待回收tid2线程 */
	err = pthread_join(tid2, &retval);
	if (err != 0) {
		err_exit(err, "can't join with thread 2");
	}
	printf("thread 2 exit code %ld\n", (long)retval);

	exit(0);
}

/*
实验：
[dendi875@192 Chapter-11-Threads]$ ./cleanup
thread 2 start
thread 2 push complete
cleanup：thread 2 second handler
cleanup：thread 2 first handler
thread 1 start
thread 1 push complete
thread 1 exit code 1
thread 2 exit code 2

从该实例中可以看到如果如果线程从它的线程函数中返回的话，它的清理处理程序就不会被调用。还有就是清理
处理程序调用的顺序和它注册的顺序时相反的

注意：有一个pthread_cleanup_push就要有一个pthread_cleanup_pop相匹配。
该程序的pthread_cleanup_pop是用来匹配pthread_cleanup_push的{的，不能省略。不然编译会出错
*/