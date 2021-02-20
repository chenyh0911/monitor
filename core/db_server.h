#pragma once
#ifndef _DB_SERVER_H
#define _DB_SERVER_H

#include "db_pool.h"

namespace DB
{
	//执行查询SQL,返回行数，结果集通过指针传输
	int select(ResultSet** result, const std::string _sql, const std::string _basedb);

	//执行增删改SQL,返回行数
	int execute(const std::string _sql, const std::string _basedb);
};


#endif // !_DB_SERVER_H
