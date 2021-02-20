#pragma once
#ifndef _SEM_LOCKER_H_
#define _SEM_LOCKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//信号量的类
class sem_locker
{
private:
	sem_t m_sem;

public:
	//初始化信号量
	sem_locker()
	{
		if (sem_init(&m_sem, 0, 0) != 0)
			printf("sem init error\n");
	};

	//销毁信号量
	~sem_locker()
	{
		sem_destroy(&m_sem);
	};

	//等待信号量
	bool wait()
	{
		return sem_wait(&m_sem) == 0;
	};

	//添加信号量
	bool add()
	{
		return sem_post(&m_sem) == 0;
	};


};

#endif