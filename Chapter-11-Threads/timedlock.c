/**
 * pthread_mutex_timedlock函数实例
 *
 *当线程试图获取一个已经加锁的互斥量时，pthread_mutex_timedlock允许指定线程的阻塞时间，
 *达到超时时间时，pthread_mutex_timedlock返回错误码ETIMEDOUT
 */

#include "apue.h"
#include <pthread.h>
#include <time.h>

int main(void)
{
	int 				err;
	struct timespec		tout;
	struct tm  			*tmp;
	char buf[64];
	pthread_mutex_t     lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);	/* 对互斥量加锁 */
	printf("mutex a locked\n");
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("current time is %s\n", buf);
	tout.tv_sec += 10;	/* 从现在起等待超过10秒 */
	// err = pthread_mutex_lock(&lock);	/* 这会造成死锁 */
	err = pthread_mutex_timedlock(&lock, &tout);
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("the time is now %s\n", buf);

	if (err == 0) {
		printf("mutex locked again!\n");
	} else {
		printf("can't lock mvtex again：%s\n", strerror(err));
	}

	exit(0);
}

/*
实验：
[dendi875@192 Chapter-11-Threads]$ ./timedlock
mutex a locked
current time is 11:45:12 PM
the time is now 11:45:22 PM
can't lock mvtex again：Connection timed out


struct timeval {
	time_t 	tv_sec;	 // 秒
	long	tv_usec; // 微秒
};

struct timespec {
	time_t	tv_sec;	 // 秒
	long	tv_nsec; // 纳秒
};
*/