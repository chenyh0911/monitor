#pragma once

#ifndef _DB_SERVER_H_
#define _DB_SERVER_H_

#include "db_pool.h"

#define CHECK_DB "CREATE DATABASE IF NOT EXISTS %s DEFAULT CHARSET utf8 COLLATE utf8_general_ci;"
#define USE_DB "use %s;"
#define CHECK_LOGGER_TABLE "CREATE TABLE IF NOT EXISTS `logger` (" \
		"`id` int(11) NOT NULL AUTO_INCREMENT," \
		"`ip` varchar(16) NOT NULL DEFAULT ''," \
		"`port` int(5) NOT NULL DEFAULT '0'," \
		"`l` tinyint(1) NOT NULL DEFAULT '0'," \
		"`level` varchar(10) NOT NULL DEFAULT ''," \
		"`time` varchar(32) NOT NULL DEFAULT ''," \
		"`log` varchar(16384) NOT NULL DEFAULT ''," \
		"PRIMARY KEY(`id`)" \
	") ENGINE = myisam CHARSET = utf8;"

#define CHECK_EVENT_TABLE "CREATE TABLE IF NOT EXISTS `event` (" \
		"`id` int(11) NOT NULL AUTO_INCREMENT," \
		"`ip` varchar(16) NOT NULL DEFAULT ''," \
		"`port` int(5) NOT NULL DEFAULT '0'," \
		"`event` tinyint(1) NOT NULL DEFAULT '1'," \
		"`time` int(11) NOT NULL DEFAULT '0'," \
		"PRIMARY KEY(`id`)" \
	") ENGINE = myisam CHARSET = utf8;"

#define INSERT_LOGGER "INSERT INTO `logger` (`ip`, `port`, `l`, `level`, `time`, `log`) " \
								"VALUES ('%s', '%d', '%d', '%s', '%s', '%s');"

#define INSERT_EVENT "INSERT INTO `event` (`ip`, `port`, `event`, `time`) " \
								"VALUES ('%s', '%d', '%d', UNIX_TIMESTAMP());"


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
