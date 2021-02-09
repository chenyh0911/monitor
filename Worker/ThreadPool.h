#pragma once
#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <list>
#include <stdio.h>
#include <exception>
#include <errno.h>
#include <pthread.h>
#include <iostream>

#include "../Locker/SemLocker.h"
#include "../Locker/MutexLocker.h"
#include "../Locker/CondLocker.h"

//线程池类（模版类）
template<class T>
class ThreadPool
{
public:
	ThreadPool(int thread_num = 20, int max_task_num = 30);
	~ThreadPool();

	//追加任务
	bool append_task(T *task);
	//初始化线程池
	void init();
	void stop();

private:
	//线程池工作,执行run
	static void *worker(void *arg);
	//线程池运行
	void run();

private:
	int thread_number;  //线程池的线程数
	int max_task_number;  //任务队列中的最大任务数
	pthread_t *all_threads;   //线程数组集合
	std::list<T *> task_queue; //任务队列
	MutexLocker queue_mutex_locker;  //互斥锁
	SemLocker queue_sem_locker;   //信号量
	bool is_stop; //是否结束线程

};



template <class T>
ThreadPool<T>::ThreadPool(int thread_num, int max_task_num) :
	thread_number(thread_num), max_task_number(max_task_num), is_stop(false), all_threads(NULL)
{
	if ((thread_num <= 0) || max_task_num <= 0)
		printf("threadpool can't init because thread_number or max_task_number is illegal\n");

	//初始化线程集合
	all_threads = new pthread_t[thread_number];
	if (!all_threads)
		printf("can't init threadpool because thread array can't new\n");
}

template <class T>
ThreadPool<T>::~ThreadPool()
{
	//销毁线程集合
	delete[]all_threads;
	is_stop = true;
}


//停止线程池
template <class T>
void ThreadPool<T>::stop()
{
	is_stop = true;
	//queue_sem_locker.add();
}

//初始化线程池
template <class T>
void ThreadPool<T>::init()
{
	for (int i = 0; i < thread_number; ++i)
	{
		printf("create the %dth pthread\n", i);
		//创建线程，线程id自增式
		//pthread_create创建线程
		//第一个参数为指向线程标识符的指针。
		//第二个参数用来设置线程属性。
		//第三个参数是线程运行函数的地址。
		//最后一个参数是运行函数的参数。
		if (pthread_create(all_threads + i, NULL, worker, this) != 0)
		{
			//创建线程失败，清除成功申请的资源并抛出异常
			delete[]all_threads;
			perror("pool thread create error");
			throw std::exception();
		};

		//设置为脱离线程
		if (pthread_detach(all_threads[i]))
		{
			//将线程设置为脱离线程，失败则清除成功申请的资源并抛出异常
			delete[]all_threads;
			perror("pool thread detach error");
			throw std::exception();
		};
	};
}

//添加任务进入任务队列
template <class T>
bool ThreadPool<T>::append_task(T *task)
{   //获取互斥锁
	queue_mutex_locker.mutex_lock();
	//判断队列中任务数是否大于最大任务数
	if (task_queue.size() > max_task_number)
	{//是则释放互斥锁
		queue_mutex_locker.mutex_unlock();
		return false;
	};

	//添加进入队列
	task_queue.push_back(task);
	queue_mutex_locker.mutex_unlock();
	//唤醒等待任务的线程
	queue_sem_locker.add();
	return true;
}

//开启工作
template <class T>
void *ThreadPool<T>::worker(void *arg)
{
	ThreadPool *pool = (ThreadPool *)arg;
	//启动线程池
	pool->run();
	return pool;
}

//启动线程池
template <class T>
void ThreadPool<T>::run()
{
	while (!is_stop)
	{
		//等待任务
		queue_sem_locker.wait();
		if (errno == EINTR)
		{
			printf("errno");
			continue;
		};

		//获取互斥锁
		queue_mutex_locker.mutex_lock();
		//判断任务队列是否为空
		if (task_queue.empty())
		{
			queue_mutex_locker.mutex_unlock();
			continue;
		};

		//获取队头任务并执行
		T *task = task_queue.front();
		task_queue.pop_front();
		queue_mutex_locker.mutex_unlock();
		if (!task)
			continue;
		//	printf("pthreadId = %ld\n", (unsigned long)pthread_self());	
		task->worker();  //worker是T对象中的方法
	}

	//停用线程池

	//测试用
	printf("close %ld\n", (unsigned long)pthread_self());
}

#endif
