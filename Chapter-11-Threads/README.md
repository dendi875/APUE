[TOC]
# Pthread库API
------------------------

## 线程函数

- pthread_create
- pthread_exit
- pthread_join
- pthread_cancel
- pthread_detach
- pthread_cleanup_push
- pthread_cleanup_pop
- pthread_self
- pthread_equal


## 线程属性函数

- 基础管理
    - pthread_attr_init
    - pthread_attr_destroy
- 分离状态属性
    - pthread_attr_getdetachstate
    - pthread_attr_setdetachstate
- 线程栈属性（地址和大小）
    - pthread_attr_getstack
    - pthread_attr_setstack
- 线程栈大小
    - pthread_attr_getstacksize
    - pthread_attr_setstacksize
- 线程栈警戒缓冲区大小
    - pthread_attr_getguardsize
    - pthread_attr_setguardsize

## 互斥量函数

- pthread_mutex_init
- pthread_mutex_destroy
- pthread_mutex_lock
- pthread_mutex_unlock
- pthread_mutex_trylock
- pthread_mutex_timedlock

## 互斥量属性函数

- 基础管理
    - pthread_mutexattr_init
    - pthread_mutexattr_destroy
- 进程共享属性
    - pthread_mutexattr_getpshared
    - pthread_mutexattr_setpshared
- 健壮属性
    - pthread_mutexattr_getrobust
    - pthread_mutexattr_setrobust
- 类型属性
    - pthread_mutexattr_gettype
    - pthread_mutexattr_settype

## 读写锁函数

- pthread_rwlock_init
- pthread_rwlock_destroy
- pthread_rwlock_rdlock
- pthread_rwlock_wrlock
- pthread_rwlock_unlock
- pthread_rwlock_tryrdlock
- pthread_rwlock_trywrlock
- pthread_rwlock_timedrdlock
- pthread_rwlock_timedwrlock

## 读写锁属性函数

- 基础管理
    - pthread_rwlockattr_init
    - pthread_rwlockattr_destroy
- 进程共享属性
    - pthread_rwlockattr_getpshared
    - pthread_rwlockattr_setpshared

## 条件变量函数

- pthread_cond_init
- pthread_cond_destroy
- pthread_cond_wait
- pthread_cond_signal
- pthread_cond_broadcast
- pthread_cond_timedwait

## 条件变量属性函数

- 基础管理
    - pthread_condattr_init
    - pthread_condattr_destroy
- 进程共享属性
    - pthread_condattr_getpshared
    - pthread_condattr_setpshared
- 时钟属性
    - pthread_condattr_getclock
    - pthread_condattr_setclock

## 自旋锁函数

- pthread_spin_init
- pthread_spin_destroy
- pthread_spin_lock
- pthread_spin_unlock
- pthread_spin_trylock

## 自旋锁属性（进程共享）

- 进程共享属性 ```pthread_spin_init```函数第二个参数
    - PTHREAD_PROCESS_SHARED
    - PTHREAD_PROCESS_PRIVATE


## 屏障函数

- pthread_barrier_init
- pthread_barrier_destroy
- pthread_barrier_wait

## 屏障属性函数

- 基础管理
    - pthread_barrierattr_init
    - pthread_barrierattr_destroy
- 进程共享属性　
    - pthread_barrierattr_getpshared
    - pthread_barrierattr_setpshared

##  参考资料

- [Parallel Computing](https://computing.llnl.gov/tutorials/parallel_comp/)
