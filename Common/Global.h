#pragma once

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "../Net/TcpSocket.h"
#include <map>
#include <string.h>

using namespace std;


//�û��ṹ����
typedef struct _CLIENT
{
	std::string ip;
	int port;
	int sockfd;

	_CLIENT()
	{
		ip = "";
		port = 0;
		sockfd = 0;
	}

} CLIENT, *PCLIENT;

//������
typedef std::map<std::string, PCLIENT> _ClientMap;
typedef std::pair<std::string, PCLIENT> _ClientPair;

//ȫ����
class Global
{
public:
	Global();
	virtual ~Global();

public:

	void SetSocket(TcpSocket* socket) { tSocket = socket; };
	TcpSocket* GetSocket() { return tSocket; };			

	//����
	static Global *getInstance()
	{
		if (!instance)
		{
			instance = new Global;
		}
		return instance;
	};								//��ȡGlobal����;

	//�����ڴ�
	static void release() 
	{
		if (instance)
		{

			delete instance->tSocket;
			//�����ڴ�
			delete instance;
		}
	};

private:
	//�洢Globalʵ��
	static Global *instance;

	TcpSocket* tSocket;  //������׽���

	//�׽��ּ���
	std::map<int, std::string> sockets;
};


#endif // !_GLOBAL_H



