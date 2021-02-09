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

#define MAX_EVENT 1024   //epoll_events的最大个数

class EpollServer
{

public:
	//初始化参数
	EpollServer(ThreadPool<BaseTask> *tPool) : is_stop(false), pool(tPool) {};
	//销毁池
	~EpollServer() { delete pool; };

	//创建epoll对象并初始化线程池
	void init();
	//启动
	void run(int);
	//清理
	void Close();

public:
	//将套接字设置成非阻塞
	static int SetNonBlock(int sockfd)  
	{
		int old_option = fcntl(sockfd, F_GETFL);
		int new_option = old_option | O_NONBLOCK;
		fcntl(sockfd, F_SETFL, new_option);
		return old_option;
	};

	//向Epoll中添加套接字
	static void addfd(int epollfd, int sockfd, bool oneshot)  
	{
		//oneshot表示是否设置称同一时刻，只能有一个线程访问fd，数据的读取都在主线程中，所以调用都设置成false
		epoll_event event;
		event.data.fd = sockfd;
		event.events = EPOLLIN | EPOLLET;
		if (oneshot)
		{
			event.events |= EPOLLONESHOT;
		};

		epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event); //添加fd
		int ret = EpollServer::SetNonBlock(sockfd);
	};

private:
	bool is_stop;   //是否停止epoll_wait的标志
	int _epollfd;    //Epoll的fd
	epoll_event events[MAX_EVENT];  //epoll的events数组

	ThreadPool<BaseTask> *pool;   //线程池指针
};

#endif

