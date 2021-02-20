#pragma once
#ifndef _TASK_H_
#define _TASK_H_


#include "base_task.h"
#include "../core/logger.h"

#define MAX_BUFFER 1024 * 16

class task : public base_task
{

public:
	//��ʼ������
	task(char *data, int fd) : _cfd(fd)
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
		logger::scheduler(_data, _cfd);
	};

private:
	int _cfd;
	std::string _data; //���ݰ�

	//int _head;	//��ͷ
	//int _len;		//���峤
	//bool _flag;	//����������ɱ�־���ֶ�����ʹ�ã�
};



#endif // !_TASK_H

