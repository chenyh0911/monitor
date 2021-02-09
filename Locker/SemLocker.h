#pragma once
#ifndef _SEM_LOCKER_H
#define _SEM_LOCKER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//�ź�������
class SemLocker
{
private:
	sem_t m_sem;

public:
	//��ʼ���ź���
	SemLocker()
	{
		if (sem_init(&m_sem, 0, 0) != 0)
			printf("sem init error\n");
	};

	//�����ź���
	~SemLocker()
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