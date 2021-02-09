#include "EpollServer.h"

//��ʼ������
void EpollServer::init()   
{
	//����Epoll����
	_epollfd = epoll_create(1024);
	if (_epollfd < 0)
	{
		perror("EpollServer epoll_create init error");
		return;
	};

}

//����
void EpollServer::run(int _sockfd)
{
	//���׽�������epoll
	addfd(_epollfd, _sockfd, false);
	while (!is_stop)
	{
		//�ȴ�ʱ�����
		int eCount = epoll_wait(_epollfd, events, MAX_EVENT, -1);
		if (eCount < 0)  //������
		{
			printf("EpollServer run epoll_wait error\n");
			break;
		};

		//printf("\nepoll event count = %d\n", eCount);

		//���������¼�
		for (int i = 0; i < eCount; ++i)
		{
			int fd = events[i].data.fd;
			//printf("epoll active event fd = %d\n", fd);
			if (fd == _sockfd)  //�µ����ӵ���
			{
				struct sockaddr_in cAddr;
				socklen_t len = sizeof(cAddr);
				int _csockfd = accept(_sockfd, (struct sockaddr *)&cAddr, &len);

				std::string client_ip(inet_ntoa(cAddr.sin_addr));    // ��ȡ�ͻ���IP
				int htons(ntohs(cAddr.sin_port));    // ��ȡ�˿ں�
				printf("fd [%d] connect %s:%d\n", _csockfd, client_ip.c_str(), htons);

				//�ͻ����׽��ּ���ܵ�
				addfd(_epollfd, _csockfd, false);
			}
			else if (events[i].events & EPOLLIN)  
			{
				//ĳ���ͻ����׽���fd�������ݿɶ����߶Ͽ�����ʱ����
				char buffer[MAX_BUFFER];
readagain:		memset(buffer, 0, sizeof(buffer));
				int ret = read(fd, buffer, MAX_BUFFER);
				if (ret == 0)  
				{
exitepoll:			
					//�û��˳�
					//Scheduler::Kickout(fd);
					//�ر��׽���
					close(fd);

					//ĳ���ͻ����׽���fd�ر������ӣ���Epoll��ɾ�����ر�fd
					struct epoll_event ev;
					ev.events = EPOLLIN;
					ev.data.fd = fd;
					epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);
					shutdown(fd, SHUT_RDWR);
					continue;
				}
				else if (ret < 0)//��ȡ���������ٴζ�ȡ
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
						//�����ѹرյĶԶ�����
						goto exitepoll;
						break;
					}
				}
				else
				{
					printf("fd [%d] recv data:%s\n", fd, buffer);
					////���ݳɹ���ȡ�����̳߳����������
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

	//����
	close(_sockfd);
	pool->stop();

};

//
void EpollServer::Close() 
{
	//�����ڴ�
	/*Scheduler::release();*/
}