#include "db_pool.h"


db_pool::db_pool(string host, string user, int port, string password, string database, int min, int max)
{
	_host = host;
	_user = user;
	_port = port;
	_password = password;
	_database = database;
	_min_size = min;
	_max_size = max;
	_now_size = 0;
	
	try
	{
		_driver = sql::mysql::get_driver_instance();
	}
	catch (sql::SQLException& e)
	{
		perror("get driver error.\n");
	}
	catch (std::runtime_error& e)
	{
		perror("[ConnPool] run time error.\n");
	}

	init_connection(_min_size);
}

db_pool::~db_pool()
{
	destory_connection();
}

Connection* db_pool::connect()
{
	Connection* conn;
	pthread_mutex_lock(&_lock);

	if (_conn_list.size() > 0)
	{
		conn = _conn_list.front();
		_conn_list.pop_front();
		if (conn->isClosed())
		{
			delete conn;
			conn = NULL;
			conn = create();
			if (conn == NULL)
			{
				--_now_size;
			}
		}

		pthread_mutex_unlock(&_lock);
		return conn;
	}
	else
	{
		if (_now_size < _max_size)
		{
			conn = create();
			if (conn)
			{
				++_now_size;
				pthread_mutex_unlock(&_lock);
				return conn;
			}
			else
			{
				pthread_mutex_unlock(&_lock);
				return NULL;
			}
		}
		else
		{
			perror("[GetConnection] connections reach the max number.");
			pthread_mutex_unlock(&_lock);
			return NULL;
		}
	}
}

void db_pool::connect(sql::Connection* conn)
{
	if (conn)
	{
		pthread_mutex_lock(&_lock);

		_conn_list.push_back(conn);

		pthread_mutex_unlock(&_lock);
	}
}

void db_pool::init_connection(int size)
{
	Connection* conn;
	pthread_mutex_lock(&_lock);

	for (int i = 0; i < size; i++)
	{
		conn = create();
		if (conn)
		{
			_conn_list.push_back(conn);
			++_now_size;
		}
		else
		{
			perror("Init connection error.");
		}
	}

	pthread_mutex_unlock(&_lock);
}

Connection* db_pool::create()
{
	Connection* conn;
	try
	{
		// 建立连接
		conn = _driver->connect(_host, _user, _password);
		return conn;
	}
	catch (sql::SQLException& e)
	{
		perror("create connection error.");
		return NULL;
	}
	catch (std::runtime_error& e)
	{
		perror("[CreateConnection] run time error.");
		return NULL;
	}
}

// 销毁连接池，需要先销毁连接池的中连接
void db_pool::destory_connection()
{
	list<Connection*>::iterator itr;
	pthread_mutex_lock(&_lock);

	for (itr = _conn_list.begin(); itr != _conn_list.end(); ++itr)
	{
		destory(*itr);
	}

	_now_size = 0;
	_conn_list.clear();

	pthread_mutex_unlock(&_lock);
}

// 销毁数据库连接
void db_pool::destory(Connection* conn)
{
	if (conn)
	{
		try
		{
			conn->close();
		}
		catch (sql::SQLException& e)
		{
			perror(e.what());
		}
		catch (std::exception& e)
		{
			perror(e.what());
		}
		delete conn;
	}
}