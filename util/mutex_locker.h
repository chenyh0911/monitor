#pragma once
#ifndef _MUTEX_LOCKER_H_
#define _MUTEX_LOCKER_H_

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

//»¥³â locker
class mutex_locker
{

public:
	//³õÊ¼»¯»¥³âÁ¿
	mutex_locker()
	{
		if (pthread_mutex_init(&m_mutex, NULL) != 0)
			printf("mutex init error!");
	};

	//Ïú»Ù»¥³âÁ¿
	~mutex_locker()
	{
		pthread_mutex_destroy(&m_mutex);
	};

	//»¥³âÁ¿Ëø
	bool mutex_lock()  //lock mutex
	{
		return pthread_mutex_lock(&m_mutex) == 0;
	};

	//»¥³âÁ¿½âËø
	bool mutex_unlock()   //unlock
	{
		return pthread_mutex_unlock(&m_mutex) == 0;
	};

private:
	pthread_mutex_t m_mutex;

};

#endif
