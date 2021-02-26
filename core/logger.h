#pragma once


#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

namespace logger
{

	//������־����
	int parse(const std::string data, std::string& level, std::string& time, std::string& log);

	//���ݵ���
	int scheduler(const std::string ip, int port, const std::string data);

};


#endif // !_LOGGER_H_


