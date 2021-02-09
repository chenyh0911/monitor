#pragma once
#ifndef _MUTEX_LOCKER_H
#define _MUTEX_LOCKER_H

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

//���� locker
class MutexLocker
{

public:
	//��ʼ��������
	MutexLocker()
	{
		if (pthread_mutex_init(&m_mutex, NULL) != 0)
			printf("mutex init error!");
	};

	//���ٻ�����
	~MutexLocker()
	{
		pthread_mutex_destroy(&m_mutex);
	};

	//��������
	bool mutex_lock()  //lock mutex
	{
		return pthread_mutex_lock(&m_mutex) == 0;
	};

	//����������
	bool mutex_unlock()   //unlock
	{
		return pthread_mutex_unlock(&m_mutex) == 0;
	};

private:
	pthread_mutex_t m_mutex;

};

#endif
