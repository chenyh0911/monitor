#pragma once
#ifndef _SEM_LOCKER_H_
#define _SEM_LOCKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//�ź�������
class sem_locker
{
private:
	sem_t m_sem;

public:
	//��ʼ���ź���
	sem_locker()
	{
		if (sem_init(&m_sem, 0, 0) != 0)
			printf("sem init error\n");
	};

	//�����ź���
	~sem_locker()
	{
		sem_destroy(&m_sem);
	};

	//�ȴ��ź���
	bool wait()
	{
		return sem_wait(&m_sem) == 0;
	};

	//����ź���
	bool add()
	{
		return sem_post(&m_sem) == 0;
	};


};

#endif