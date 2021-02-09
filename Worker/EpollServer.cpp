#include "EpollServer.h"

//初始化服务
void EpollServer::init()   
{
	//创建Epoll对象
	_epollfd = epoll_create(1024);
	if (_epollfd < 0)
	{
		perror("EpollServer epoll_create init error");
		return;
	};

}

//启动
void EpollServer::run(int _sockfd)
{
	//把套接字置入epoll
	addfd(_epollfd, _sockfd, false);
	while (!is_stop)
	{
		//等待时间产生
		int eCount = epoll_wait(_epollfd, events, MAX_EVENT, -1);
		if (eCount < 0)  //出错处理
		{
			printf("EpollServer run epoll_wait error\n");
			break;
		};

		//printf("\nepoll event count = %d\n", eCount);

		//遍历处理事件
		for (int i = 0; i < eCount; ++i)
		{
			int fd = events[i].data.fd;
			//printf("epoll active event fd = %d\n", fd);
			if (fd == _sockfd)  //新的连接到来
			{
				struct sockaddr_in cAddr;
				socklen_t len = sizeof(cAddr);
				int _csockfd = accept(_sockfd, (struct sockaddr *)&cAddr, &len);

				std::string client_ip(inet_ntoa(cAddr.sin_addr));    // 获取客户端IP
				int htons(ntohs(cAddr.sin_port));    // 获取端口号
				printf("fd [%d] connect %s:%d\n", _csockfd, client_ip.c_str(), htons);

				//客户端套接字加入管道
				addfd(_epollfd, _csockfd, false);
			}
			else if (events[i].events & EPOLLIN)  
			{
				//某个客户端套接字fd上有数据可读或者断开连接时触发
				char buffer[MAX_BUFFER];
readagain:		memset(buffer, 0, sizeof(buffer));
				int ret = read(fd, buffer, MAX_BUFFER);
				if (ret == 0)  
				{
exitepoll:			
					//用户退出
					//Scheduler::Kickout(fd);
					//关闭套接字
					close(fd);

					//某个客户端套接字fd关闭了连接，从Epoll中删除并关闭fd
					struct epoll_event ev;
					ev.events = EPOLLIN;
					ev.data.fd = fd;
					epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);
					shutdown(fd, SHUT_RDWR);
					continue;
				}
				else if (ret < 0)//读取出错，尝试再次读取
				{
					printf("fd [%d] ret = %d again, errno = %d\n", fd, ret, errno);
					if (errno == EAGAIN)
					{
						printf("read error! read again\n");
						goto readagain;
						break;
					}
					else if (errno == 104)
					{
						//接收已关闭的对端数据
						goto exitepoll;
						break;
					}
				}
				else
				{
					printf("fd [%d] recv data:%s\n", fd, buffer);
					////数据成功读取，向线程池中添加任务
					//BaseTask *task = new Task(buffer, fd);
					//pool->append_task(task);
				}
			}
			else
			{
				printf("something else had happened\n");
			}
		}
	}

	//结束
	close(_sockfd);
	pool->stop();

};

//
void EpollServer::Close() 
{
	//清理内存
	/*Scheduler::release();*/
}