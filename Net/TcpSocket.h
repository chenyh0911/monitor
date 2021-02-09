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
	//���캯��
	TcpSocket(int rPort) : _sockfd(-1), port(rPort) {};
	//��������
	~TcpSocket();
	//�ر��׽���
	bool Close() { close(_sockfd); _sockfd = -1; };

	//�����׽���
	void SetSockFd(int fd) { _sockfd = fd; };
	//��ȡ�׽���
	int GetSockFd() { return _sockfd; };

	//���׽���ת����NET
	bool ConvertToNet(int _sockfd, std::string &ip, int &port);

	//�����׽���
	bool init();
	//�󶨶˿�
	bool Bind();
	//�����׽���
	bool Listen(int backlog = 10);

private:
	int _sockfd; //������׽���
	int port; //�󶨶˿�
};


#endif // !_TCP_SOCKET_H
