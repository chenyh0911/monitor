#pragma once
#ifndef _DB_SERVER_H
#define _DB_SERVER_H

#include "db_pool.h"

namespace DB
{
	//ִ�в�ѯSQL,���������������ͨ��ָ�봫��
	int select(ResultSet** result, const std::string _sql, const std::string _basedb);

	//ִ����ɾ��SQL,��������
	int execute(const std::string _sql, const std::string _basedb);
};


#endif // !_DB_SERVER_H
