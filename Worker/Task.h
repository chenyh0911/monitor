#pragma once
#ifndef _TASK_H_
#define _TASK_H_


#include "base_task.h"
#include "../core/logger.h"

#define MAX_BUFFER 1024 * 16

class task : public base_task
{

public:
	//初始化任务
	task(char *data, int fd) : _cfd(fd)
	{
		//解析包
		////结构：包头+包体长度+包体
		////包头4字节
		//memcpy(&_head, &data[0], 4);
		//memcpy(&_len, &data[4], 4);

		_data = data;
	};

	//任务执行
	void worker()
	{
		logger::scheduler(_data, _cfd);
	};

private:
	int _cfd;
	std::string _data; //数据包

	//int _head;	//包头
	//int _len;		//包体长
	//bool _flag;	//接收数据完成标志（分段数据使用）
};



#endif // !_TASK_H

