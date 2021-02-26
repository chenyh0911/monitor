#include "epoll_server.h"

epoll_server::epoll_server(server* _server, thread_pool<base_task>* p)
	: _server(_server), _pool(p), _is_stop(false)
{

}

epoll_server::~epoll_server() 
{
	delete _pool;
}

int epoll_server::init()   
{
	_epoll_fd = epoll_create(MAX_EVENT);
	if (_epoll_fd < 0)
		return -1;

	return 0;
}

int epoll_server::run()
{
	//���׽�������epoll
	add_fd(_epoll_fd, _server->fd(), false);
	while (!_is_stop)
	{
		int count = epoll_wait(_epoll_fd, events, MAX_EVENT, -1);
		if (count < 0) 
		{
			printf("epoll server run epoll_wait error\n");
			break;
		};

		for (int i = 0; i < count; ++i)
		{
			int fd = events[i].data.fd;
			if (fd == _server->fd())
			{
				int cfd = _server->insert_client();
				add_fd(_epoll_fd, cfd, false);
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
					//�û��˳�,�ر��׽���
					struct epoll_event ev;
					ev.events = EPOLLIN;
					ev.data.fd = fd;
					epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, &ev);
					shutdown(fd, SHUT_RDWR);

					_server->remove_client(fd);
					continue;
				}
				else if (ret < 0)
				{
					//��ȡ���������ٴζ�ȡ
					if (errno == EAGAIN)
					{
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
					//���ݳɹ���ȡ�����̳߳����������
					std::string client_ip;
					int client_port;
					if (_server->convert_to_net(fd, client_ip, client_port) == -1)
					{
						std::cout << "convert to client ip/port error" << std::endl;
						client_ip = "0.0.0.0";
						client_port = 0;
					};

					base_task *t = new task(client_ip.c_str(), client_port, buffer);
					_pool->task(t);
				}
			}
			else
			{
				printf("something else had happened\n");
			}
		}
	}

	_pool->stop();
	return 0;
};
