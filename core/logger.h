#pragma once


#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

namespace logger
{

	//解析日志数据
	int parse(const char*, std::string&, std::string&, std::string&);

	//数据调度
	int scheduler(const std::string, int);

};


#endif // !_LOGGER_H_


