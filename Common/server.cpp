#include "server.h"

#include "../common/common.h"

server::server(int port, int backlog) 
	: _fd(-1), _port(port)
{
	_backlog = (backlog > 5) ? backlog : 5;
}

server::~server()
{
	clear_client();
	release();
};

int server::init() 
{
	_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_fd < 0)
	{
		perror("socket error\n");
		return -1;
	}

	//int opt = 1;
	//setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//开启keepalive

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t len = sizeof(struct sockaddr_in);
	//memset(&addr, 0, sizeof(addr)); //数据初始化--清零

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	//addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(_port);

	if (bind(_fd, (struct sockaddr*)&addr, len) < 0)
	{
		perror("bind error");
		return -1;
	}

	return 0;
}

int server::run()
{
	//int listen(int sockfd, int backlog);
	//backlog:最大并发连接数--内核中已完成连接队列的最大节点数
	int ret = listen(_fd, _backlog);
	if (ret < 0)
	{
		perror("listen error");
		return -1;
	}

	return 0;
};

int server::insert_client()
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int cfd = accept(_fd, (struct sockaddr*)&addr, &len);
	if (cfd == -1)
		return -1;

	std::string client_ip(inet_ntoa(addr.sin_addr));
	int client_port(ntohs(addr.sin_port));

	//创建目录
	pconfig pconf = global::init()->config();
	char path[512];
	memset(path, '\0', sizeof path);
	snprintf(path, sizeof(path), "%s/%s", pconf->f_dir.c_str(), client_ip.c_str());

	pclient c = new client;
	c->fd = cfd;
	c->ip = client_ip.c_str();
	c->port = client_port;
	c->path = path;
	
	if (common::create_dir(path) == -1)
	{
		printf("[%s:%d] create dir error, fd=%d\n", client_ip.c_str(), client_port, cfd);
		c->path = "";
	}
	else 
	{
		common::connect_in_or_out(c, 1);
	}

	_clients.insert(std::pair<int, pclient>(cfd, c));
	printf("[%s:%d] connected successfully, fd=%d, current size=%d\n", client_ip.c_str(), client_port, cfd, _clients.size());
	return cfd;
}

int server::remove_client(int cfd)
{
	std::map<int, pclient>::iterator itr = _clients.find(cfd);
	if (_clients.end() == itr)
		return -1;
	
	common::connect_in_or_out(itr->second, 0);

	char ip[16];
	memset(ip, '\0', sizeof ip);
	memcpy(ip, itr->second->ip.c_str(), itr->second->ip.length());

	delete itr->second;
	printf("[%s:%d] disconnect, fd=%d, remain size=%d\n", itr->second->ip.c_str(), itr->second->port, cfd, _clients.size());
	_clients.erase(cfd);

	if (!is_exist(ip))
		global::init()->del_ip_file(ip);

	return 0;
}

pclient server::find_client(int cfd)
{
	std::map<int, pclient>::iterator itr = _clients.find(cfd);
	if (_clients.end() == itr)
		return NULL;

	return itr->second;
}

int server::clear_client() 
{
	std::map<int, pclient>::iterator iter = _clients.begin();
	while (iter != _clients.end())
	{
		//清理
		delete iter->second;
		iter++;
	}

	_clients.clear();
	return 0;
}

int server::is_exist(const char* ip)
{
	std::map<int, pclient>::iterator itr = _clients.begin();
	while (itr != _clients.end())
	{
		if (strcmp(itr->second->ip.c_str(), ip) == 0)
		{
			return 1;
		};
	}

	return 0;
}

int server::convert_to_net(int cfd, std::string &ip, int &port)
{
	struct sockaddr_in client;//分别表示连接的对端地址即客户端地址
	char addr[INET_ADDRSTRLEN];//保存点分十进制的地址
	socklen_t len;
	len = sizeof(client);

	if (getpeername(cfd, (struct sockaddr *)&client, &len) == -1)
	{
		printf("fd [%d] getpeername error\n", cfd);
		return -1;
	}

	ip = inet_ntop(AF_INET, &client.sin_addr, addr, sizeof(addr));
	port = ntohs(client.sin_port);
	//printf("connected peer address = %s:%d\n", ip, ntohs(cAddr.sin_port));
	return 0;
}