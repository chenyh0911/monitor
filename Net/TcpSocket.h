#pragma once
#ifndef _TCP_SOCKET_H
#define _TCP_SOCKET_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <map>

using namespace std;

class TcpSocket
{

public:
	//构造函数
	TcpSocket(int rPort) : _sockfd(-1), port(rPort) {};
	//析构函数
	~TcpSocket();
	//关闭套接字
	bool Close() { close(_sockfd); _sockfd = -1; };

	//更新套接字
	void SetSockFd(int fd) { _sockfd = fd; };
	//获取套接字
	int GetSockFd() { return _sockfd; };

	//用套接字转换成NET
	bool ConvertToNet(int _sockfd, std::string &ip, int &port);

	//创建套接字
	bool init();
	//绑定端口
	bool Bind();
	//监听套接字
	bool Listen(int backlog = 10);

private:
	int _sockfd; //服务端套接字
	int port; //绑定端口
};


#endif // !_TCP_SOCKET_H
