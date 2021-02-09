#include "TcpSocket.h"

//析构函数
TcpSocket::~TcpSocket()
{
	//关闭服务端套接字
	Close();
};

//创建套接字
bool TcpSocket::init() 
{
	_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sockfd < 0)
	{
		perror("socket error\n");
		return false;
	}

	//int opt = 1;
	//setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//开启keepalive
	return true;
}

//绑定套接字结构体
bool TcpSocket::Bind()
{
	struct sockaddr_in addr;
	//memset(&addr, 0, sizeof(addr)); //数据初始化--清零
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET; //设置IP通讯
	addr.sin_addr.s_addr = INADDR_ANY; //服务器IP地址-允许连接到所有本地地址上"0.0.0.0"
	//addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port); //服务器端口号

	socklen_t len = sizeof(struct sockaddr_in);
	int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
	if (ret < 0)
	{
		perror("bind error");
		return false;
	}
	return true;
};

//监听套接字
bool TcpSocket::Listen(int backlog)
{
	//int listen(int sockfd, int backlog);
	//backlog:最大并发连接数--内核中已完成连接队列的最大节点数
	int ret = listen(_sockfd, backlog);
	if (ret < 0)
	{
		perror("listen error");
		return false;
	}
	return  true;
};

bool TcpSocket::ConvertToNet(int _sockfd, std::string &ip, int &port)
{
	struct sockaddr_in cAddr;//分别表示连接的对端地址即客户端地址
	char ipAddr[INET_ADDRSTRLEN];//保存点分十进制的地址
	socklen_t clen;
	//获取connfd表示的连接上的对端地址
	clen = sizeof(cAddr);
	if (getpeername(_sockfd, (struct sockaddr *)&cAddr, &clen) == -1) 
	{
		printf("fd [%d] getpeername error\n", _sockfd);
		return false;
	}
	ip = inet_ntop(AF_INET, &cAddr.sin_addr, ipAddr, sizeof(ipAddr));
	port = ntohs(cAddr.sin_port);
	//printf("connected peer address = %s:%d\n", ip, ntohs(cAddr.sin_port));
	return true;
}