#pragma once
#ifndef _TASK_H
#define _TASK_H

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

#include "BaseTask.h"

#define MAX_BUFFER 128

class Task : public BaseTask
{

public:
	//初始化任务
	Task(char *data, int fd) : _sockfd(fd)
	{
		//解析包
		//结构：包头+包体长度+包体
		//包头4字节
		memcpy(&_head, &data[0], 4);
		memcpy(&_len, &data[4], 4);

		char idata[MAX_BUFFER - 8];
		memset(idata, '\0', sizeof idata);
		memcpy(&idata, &data[8], sizeof idata);

		_data = idata;
		_flag = (_data.length() == _len) ? 1 : 0;
	};

	//任务执行
	void worker()
	{
		if (_flag == 0)
		{
			//接收未完成
			return;
		}

		//数据分析
		std::cout << _data << std::endl;
	};

private:
	int _sockfd; //客户端套接字

	bool _flag;	//接收数据完成标志
	int _head;	//包头
	int _len;		//包体长
	std::string _data; //工作订单
};



#endif // !_TASK_H

