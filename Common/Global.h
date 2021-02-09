#pragma once

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "../Net/TcpSocket.h"
#include <map>
#include <string.h>

using namespace std;


//用户结构定义
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

//重命名
typedef std::map<std::string, PCLIENT> _ClientMap;
typedef std::pair<std::string, PCLIENT> _ClientPair;

//全局类
class Global
{
public:
	Global();
	virtual ~Global();

public:

	void SetSocket(TcpSocket* socket) { tSocket = socket; };
	TcpSocket* GetSocket() { return tSocket; };			

	//单例
	static Global *getInstance()
	{
		if (!instance)
		{
			instance = new Global;
		}
		return instance;
	};								//获取Global对象;

	//清理内存
	static void release() 
	{
		if (instance)
		{

			delete instance->tSocket;
			//清理内存
			delete instance;
		}
	};

private:
	//存储Global实例
	static Global *instance;

	TcpSocket* tSocket;  //服务端套接字

	//套接字集合
	std::map<int, std::string> sockets;
};


#endif // !_GLOBAL_H



