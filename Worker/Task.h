#pragma once
#ifndef _TASK_H_
#define _TASK_H_


#include "base_task.h"
#include "../core/logger.h"
#include "../util/ini.h"

#define MAX_BUFFER 1024 * 16

class task : public base_task
{

public:
	//初始化任务
	task(int cfd, const char* data) 
		: _cfd(cfd)
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
		if (trim(_data).size() > 0)
		{
			logger::scheduler(_cfd, _data);
		};
	};

private:
	
	std::string& trim(std::string& str)
	{
		if (str.empty())
			return str;

		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
		return str;
	}

private:

	int _cfd; //客户端套接字
	std::string _data; //数据包

	//int _head;	//包头
	//int _len;		//包体长
	//bool _flag;	//接收数据完成标志（分段数据使用）
};



#endif // !_TASK_H

