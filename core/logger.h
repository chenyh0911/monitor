#pragma once


#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>

#define MAX_BUFFER 1024 * 16

typedef struct _log {

	char time[32];
	char level[8];
	char data[MAX_BUFFER];

	_log()
	{
		memset(time, '\0', sizeof time);
		memset(level, '\0', sizeof level);
		memset(data, '\0', sizeof data);
	}

} llog, * plog;


namespace logger
{

	//解析日志数据
	int parse(const std::string buffer, std::vector<plog>& logs);

	//数据调度
	int scheduler(int _cfd, const std::string buffer);

	//写入日志文本
	int log(const std::string ip, int port, const std::string buffer);
};


#endif // !_LOGGER_H_


