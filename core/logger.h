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
	int parse(const char*, std::string&, std::string&, std::string&);

	//���ݵ���
	int scheduler(const std::string, int);

};


#endif // !_LOGGER_H_


