#pragma once

#ifndef _DB_SERVER_H_
#define _DB_SERVER_H_

#include "db_pool.h"

namespace DB
{

	//检测数据库是否存在
	int check(const std::string);

	//执行查询SQL,返回行数，结果集通过指针传输
	int select(ResultSet**, const std::string, const std::string);

	//执行增删改SQL,返回行数
	int execute(const std::string, const std::string);
};


#endif // !_DB_SERVER_H_
