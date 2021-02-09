#pragma once
#ifndef _COND_LOCKER_H
#define _COND_LOCKER_H

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>


//�������� locker
class CondLocker
{

public:
	// ��ʼ�� m_mutex and m_cond
	CondLocker()
	{
		if (pthread_mutex_init(&m_mutex, NULL) != 0)
			printf("mutex init error");

		if (pthread_cond_init(&m_cond, NULL) != 0)
		{   //����������ʼ���Ǳ����ͷų�ʼ���ɹ���mutex
			pthread_mutex_destroy(&m_mutex);
			printf("cond init error");
		}
	};

	// ���� mutex and cond
	~CondLocker()
	{
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
	};

	//�ȴ���������
	bool wait()
	{
		int ans = 0;
		pthread_mutex_lock(&m_mutex);
		ans = pthread_cond_wait(&m_cond, &m_mutex);
		pthread_mutex_unlock(&m_mutex);
		return ans == 0;
	};

	//���ѵȴ������������߳�
	bool signal()
	{
		return pthread_cond_signal(&m_cond) == 0;
	};

private:
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;

};

#endif
