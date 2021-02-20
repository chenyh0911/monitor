#pragma once

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <list>
#include <stdio.h>
#include <exception>
#include <errno.h>
#include <pthread.h>
#include <iostream>

#include "../util/sem_locker.h"
#include "../util/mutex_locker.h"
#include "../util/cond_locker.h"


template<class T>
class thread_pool
{
public:
	thread_pool(int, int);
	~thread_pool();

public:
	int init();
	int stop();
	int task(T*);

private:
	static void *worker(void*);
	void run();

private:
	bool _is_stop; //是否结束线程
	int _thread_number;  //线程池的线程数
	int _max_task_number;  //任务队列中的最大任务数

	pthread_t *_all_threads;   //线程数组集合
	std::list<T*> _task_queue; //任务队列

	mutex_locker _queue_mutex_locker;  //互斥锁
	sem_locker _queue_sem_locker;   //信号量

};


template <class T>
thread_pool<T>::thread_pool(int thread_num, int max_task_num) :
	_is_stop(false), _all_threads(NULL)
{
	_thread_number = (thread_num > 5) ? thread_num : 5;
	_max_task_number = (max_task_num > 20) ? max_task_num : 20;
}

template <class T>
thread_pool<T>::~thread_pool()
{
	delete[] _all_threads;
	_is_stop = true;
}

template <class T>
int thread_pool<T>::stop()
{
	_is_stop = true;
	//queue_sem_locker.add();
	return 0;
}

template <class T>
int thread_pool<T>::init()
{
	_all_threads = new pthread_t[_thread_number];
	if (!_all_threads)
		return -1;
	
	for (int i = 0; i < _thread_number; ++i)
	{
		if (pthread_create(_all_threads + i, NULL, worker, this) != 0)
		{
			delete[] _all_threads;
			return -1;
		};

		//将线程设置为脱离线程，失败则清除成功申请的资源并抛出异常
		if (pthread_detach(_all_threads[i]))
		{
			delete[] _all_threads;
			//throw std::exception();			
			return -1;
		};
	};

	return 0;
}

template <class T>
int thread_pool<T>::task(T *t)
{
	_queue_mutex_locker.mutex_lock();
	if (_task_queue.size() > _max_task_number)
	{
		_queue_mutex_locker.mutex_unlock();
		return -1;
	};

	_task_queue.push_back(t);
	_queue_mutex_locker.mutex_unlock();
	//唤醒等待任务的线程
	_queue_sem_locker.add();
	return 0;
}

template <class T>
void * thread_pool<T>::worker(void *arg)
{
	thread_pool *pool = (thread_pool*)arg;
	pool->run();
	return pool;
}

template <class T>
void thread_pool<T>::run()
{
	while (!_is_stop)
	{
		_queue_sem_locker.wait();
		if (errno == EINTR)
			continue;

		_queue_mutex_locker.mutex_lock();
		if (_task_queue.empty())
		{
			_queue_mutex_locker.mutex_unlock();
			continue;
		};

		T *task = _task_queue.front();
		_task_queue.pop_front();
		_queue_mutex_locker.mutex_unlock();
		if (!task)
			continue;

		task->worker();
	}

	printf("close %ld\n", (unsigned long)pthread_self());
}

#endif
