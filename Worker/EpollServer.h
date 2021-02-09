#pragma once
#ifndef _EPOLL_SERVER_H_
#define _EPOLL_SERVER_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>
//#include <pthread.h>

#include "ThreadPool.h"
#include "BaseTask.h"
#include "Task.h"

#define MAX_EVENT 1024   //epoll_events��������

class EpollServer
{

public:
	//��ʼ������
	EpollServer(ThreadPool<BaseTask> *tPool) : is_stop(false), pool(tPool) {};
	//���ٳ�
	~EpollServer() { delete pool; };

	//����epoll���󲢳�ʼ���̳߳�
	void init();
	//����
	void run(int);
	//����
	void Close();

public:
	//���׽������óɷ�����
	static int SetNonBlock(int sockfd)  
	{
		int old_option = fcntl(sockfd, F_GETFL);
		int new_option = old_option | O_NONBLOCK;
		fcntl(sockfd, F_SETFL, new_option);
		return old_option;
	};

	//��Epoll������׽���
	static void addfd(int epollfd, int sockfd, bool oneshot)  
	{
		//oneshot��ʾ�Ƿ����ó�ͬһʱ�̣�ֻ����һ���̷߳���fd�����ݵĶ�ȡ�������߳��У����Ե��ö����ó�false
		epoll_event event;
		event.data.fd = sockfd;
		event.events = EPOLLIN | EPOLLET;
		if (oneshot)
		{
			event.events |= EPOLLONESHOT;
		};

		epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event); //���fd
		int ret = EpollServer::SetNonBlock(sockfd);
	};

private:
	bool is_stop;   //�Ƿ�ֹͣepoll_wait�ı�־
	int _epollfd;    //Epoll��fd
	epoll_event events[MAX_EVENT];  //epoll��events����

	ThreadPool<BaseTask> *pool;   //�̳߳�ָ��
};

#endif

