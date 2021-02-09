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

//�̳߳��ࣨģ���ࣩ
template<class T>
class ThreadPool
{
public:
	ThreadPool(int thread_num = 20, int max_task_num = 30);
	~ThreadPool();

	//׷������
	bool append_task(T *task);
	//��ʼ���̳߳�
	void init();
	void stop();

private:
	//�̳߳ع���,ִ��run
	static void *worker(void *arg);
	//�̳߳�����
	void run();

private:
	int thread_number;  //�̳߳ص��߳���
	int max_task_number;  //��������е����������
	pthread_t *all_threads;   //�߳����鼯��
	std::list<T *> task_queue; //�������
	MutexLocker queue_mutex_locker;  //������
	SemLocker queue_sem_locker;   //�ź���
	bool is_stop; //�Ƿ�����߳�

};



template <class T>
ThreadPool<T>::ThreadPool(int thread_num, int max_task_num) :
	thread_number(thread_num), max_task_number(max_task_num), is_stop(false), all_threads(NULL)
{
	if ((thread_num <= 0) || max_task_num <= 0)
		printf("threadpool can't init because thread_number or max_task_number is illegal\n");

	//��ʼ���̼߳���
	all_threads = new pthread_t[thread_number];
	if (!all_threads)
		printf("can't init threadpool because thread array can't new\n");
}

template <class T>
ThreadPool<T>::~ThreadPool()
{
	//�����̼߳���
	delete[]all_threads;
	is_stop = true;
}


//ֹͣ�̳߳�
template <class T>
void ThreadPool<T>::stop()
{
	is_stop = true;
	//queue_sem_locker.add();
}

//��ʼ���̳߳�
template <class T>
void ThreadPool<T>::init()
{
	for (int i = 0; i < thread_number; ++i)
	{
		printf("create the %dth pthread\n", i);
		//�����̣߳��߳�id����ʽ
		//pthread_create�����߳�
		//��һ������Ϊָ���̱߳�ʶ����ָ�롣
		//�ڶ����������������߳����ԡ�
		//�������������߳����к����ĵ�ַ��
		//���һ�����������к����Ĳ�����
		if (pthread_create(all_threads + i, NULL, worker, this) != 0)
		{
			//�����߳�ʧ�ܣ�����ɹ��������Դ���׳��쳣
			delete[]all_threads;
			perror("pool thread create error");
			throw std::exception();
		};

		//����Ϊ�����߳�
		if (pthread_detach(all_threads[i]))
		{
			//���߳�����Ϊ�����̣߳�ʧ��������ɹ��������Դ���׳��쳣
			delete[]all_threads;
			perror("pool thread detach error");
			throw std::exception();
		};
	};
}

//�����������������
template <class T>
bool ThreadPool<T>::append_task(T *task)
{   //��ȡ������
	queue_mutex_locker.mutex_lock();
	//�ж϶������������Ƿ�������������
	if (task_queue.size() > max_task_number)
	{//�����ͷŻ�����
		queue_mutex_locker.mutex_unlock();
		return false;
	};

	//��ӽ������
	task_queue.push_back(task);
	queue_mutex_locker.mutex_unlock();
	//���ѵȴ�������߳�
	queue_sem_locker.add();
	return true;
}

//��������
template <class T>
void *ThreadPool<T>::worker(void *arg)
{
	ThreadPool *pool = (ThreadPool *)arg;
	//�����̳߳�
	pool->run();
	return pool;
}

//�����̳߳�
template <class T>
void ThreadPool<T>::run()
{
	while (!is_stop)
	{
		//�ȴ�����
		queue_sem_locker.wait();
		if (errno == EINTR)
		{
			printf("errno");
			continue;
		};

		//��ȡ������
		queue_mutex_locker.mutex_lock();
		//�ж���������Ƿ�Ϊ��
		if (task_queue.empty())
		{
			queue_mutex_locker.mutex_unlock();
			continue;
		};

		//��ȡ��ͷ����ִ��
		T *task = task_queue.front();
		task_queue.pop_front();
		queue_mutex_locker.mutex_unlock();
		if (!task)
			continue;
		//	printf("pthreadId = %ld\n", (unsigned long)pthread_self());	
		task->worker();  //worker��T�����еķ���
	}

	//ͣ���̳߳�

	//������
	printf("close %ld\n", (unsigned long)pthread_self());
}

#endif
