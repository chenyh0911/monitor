#pragma once

#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <string.h>
#include <map>

typedef struct _CLIENT
{
	std::string ip;
	int port;
	int fd;
	std::string path;

	_CLIENT()
	{
		ip = "";
		port = 0;
		fd = 0;
		path = "";
	}

} client, *pclient;

class server
{

public:
	server(int, int);
	virtual ~server();
	
public:
	void fd(int fd) { _fd = fd; };
	int fd() { return _fd; };

	void clients(std::map<int, pclient> cs) { _clients = cs; };
	std::map<int, pclient> clients() { return _clients; };

public:
	int init();
	int run();

public:
	int insert_client();
	int remove_client(int cfd); 
	pclient find_client(int cfd);
	int clear_client();
	int is_exist(const char* ip);
	void release() { close(_fd); _fd = -1; };

public:
	static int convert_to_net(int cfd, std::string& ip, int& port);

private:
	int _fd;
	int _port; 
	int _backlog;

	std::map<int, pclient> _clients;
};


#endif // !_SERVER_H_
