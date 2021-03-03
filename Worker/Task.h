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
	//��ʼ������
	task(int cfd, const char* data) 
		: _cfd(cfd)
	{
		//������
		////�ṹ����ͷ+���峤��+����
		////��ͷ4�ֽ�
		//memcpy(&_head, &data[0], 4);
		//memcpy(&_len, &data[4], 4);

		_data = data;
	};

	//����ִ��
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

	int _cfd; //�ͻ����׽���
	std::string _data; //���ݰ�

	//int _head;	//��ͷ
	//int _len;		//���峤
	//bool _flag;	//����������ɱ�־���ֶ�����ʹ�ã�
};



#endif // !_TASK_H

