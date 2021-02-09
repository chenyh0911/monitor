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
	//��ʼ������
	Task(char *data, int fd) : _sockfd(fd)
	{
		//������
		//�ṹ����ͷ+���峤��+����
		//��ͷ4�ֽ�
		memcpy(&_head, &data[0], 4);
		memcpy(&_len, &data[4], 4);

		char idata[MAX_BUFFER - 8];
		memset(idata, '\0', sizeof idata);
		memcpy(&idata, &data[8], sizeof idata);

		_data = idata;
		_flag = (_data.length() == _len) ? 1 : 0;
	};

	//����ִ��
	void worker()
	{
		if (_flag == 0)
		{
			//����δ���
			return;
		}

		//���ݷ���
		std::cout << _data << std::endl;
	};

private:
	int _sockfd; //�ͻ����׽���

	bool _flag;	//����������ɱ�־
	int _head;	//��ͷ
	int _len;		//���峤
	std::string _data; //��������
};



#endif // !_TASK_H

