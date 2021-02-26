#pragma once

#ifndef _DB_SERVER_H_
#define _DB_SERVER_H_

#include "db_pool.h"

namespace DB
{

	//������ݿ��Ƿ����
	int check(const std::string);

	//ִ�в�ѯSQL,���������������ͨ��ָ�봫��
	int select(ResultSet**, const std::string, const std::string);

	//ִ����ɾ��SQL,��������
	int execute(const std::string, const std::string);
};


#endif // !_DB_SERVER_H_
