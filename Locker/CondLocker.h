#pragma once
#ifndef _COND_LOCKER_H
#define _COND_LOCKER_H

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>


//条件变量 locker
class CondLocker
{

public:
	// 初始化 m_mutex and m_cond
	CondLocker()
	{
		if (pthread_mutex_init(&m_mutex, NULL) != 0)
			printf("mutex init error");

		if (pthread_cond_init(&m_cond, NULL) != 0)
		{   //条件变量初始化是被，释放初始化成功的mutex
			pthread_mutex_destroy(&m_mutex);
			printf("cond init error");
		}
	};

	// 销毁 mutex and cond
	~CondLocker()
	{
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
	};

	//等待条件变量
	bool wait()
	{
		int ans = 0;
		pthread_mutex_lock(&m_mutex);
		ans = pthread_cond_wait(&m_cond, &m_mutex);
		pthread_mutex_unlock(&m_mutex);
		return ans == 0;
	};

	//唤醒等待条件变量的线程
	bool signal()
	{
		return pthread_cond_signal(&m_cond) == 0;
	};

private:
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;

};

#endif
