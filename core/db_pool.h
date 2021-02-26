#pragma once
#ifndef _DB_CONNECTION_POOL_H_
#define _DB_CONNECTION_POOL_H_

#include <stdexcept>
#include <exception>
#include <stdio.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <pthread.h>
#include <list>
#include <string>

using namespace std;
using namespace sql;


class db_pool
{
public:
	db_pool(string, string, string, string, int, int);
	virtual ~db_pool();

public:
	Connection* connect();
	void connect(Connection* conn);

private:
	void init_connection(int);
	void destory_connection();

	Connection* create();
	void destory(Connection* conn);

private:

	std::string _host;
	std::string _user;
	std::string _password;

	int _min_size;
	int _max_size;
	int _now_size;

	list<Connection*> _conn_list;

	Driver* _driver;
	pthread_mutex_t _lock;

	static db_pool* _pool;

};

#endif /*_DB_CONNECTION_POOL_H_ */


