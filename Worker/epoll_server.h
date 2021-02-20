#pragma once
#ifndef _EPOLL_SERVER_H_
#define _EPOLL_SERVER_H_

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include "base_task.h"
#include "task.h"
#include "thread_pool.h"
#include "../common/server.h"

#define MAX_EVENT 1024   //epoll_events的最大个数

class epoll_server
{

public:
	epoll_server(server*, thread_pool<base_task>*);
	virtual ~epoll_server();

public:
	int init();
	int run();

public:
	//将套接字设置成非阻塞
	static int set_non_block(int sockfd)  
	{
		int old_option = fcntl(sockfd, F_GETFL);
		int new_option = old_option | O_NONBLOCK;
		fcntl(sockfd, F_SETFL, new_option);
		return old_option;
	};

	//向Epoll中添加套接字
	static void add_fd(int epollfd, int sockfd, bool oneshot)  
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
		epoll_server::set_non_block(sockfd);
	};

private:
	bool _is_stop;  
	int _epoll_fd; 

	epoll_event events[MAX_EVENT];
	thread_pool<base_task> *_pool; 
	server* _server;
};

#endif

