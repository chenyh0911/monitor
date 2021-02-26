#include "db_pool.h"

#include <string.h>
#include <exception>


db_pool::db_pool(string host, string user, string port, string password, int min, int max)
{
	/*_host.append("tcp://");
	_host.append(host.c_str());
	_host.append(":");
	_host.append(port.c_str());*/

	int l1 = sizeof("tcp://") - 1;
	int l2 = strlen(host.c_str()) - 1;
	int l3 = sizeof(":") - 1;
	int l4 = strlen(port.c_str()) - 1;

	char h[64];
	//memset(h, '\0', sizeof h);
	memcpy(h, "tcp://", l1);
	memcpy(h + l1, host.c_str(), l2);
	memcpy(h + l1 + l2, ":", l3);
	memcpy(h + l1 + l2 + l3, port.c_str(), l4);
	h[l1 + l2 + l3 + l4] = '\0';

	char u[10];
	memcpy(u, user.c_str(), strlen(user.c_str()) - 1);
	u[strlen(user.c_str()) - 1] = '\0';
	
	char p[32];
	memcpy(p, password.c_str(), strlen(password.c_str()) - 1);
	p[strlen(password.c_str()) - 1] = '\0';

	_host = h;
	_user = u;
	_password = p;
	_min_size = min;
	_max_size = max;
	_now_size = 0;

	try
	{
		_driver = sql::mysql::get_driver_instance();
	}
	catch (sql::SQLException& e)
	{
		std::cout << "get driver instance error:" << e.what() << std::endl;
	}
	catch (std::runtime_error& e)
	{
		//perror("[ConnPool] run time error.\n");
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
			std::cout << "mysql disconnect, please check db config..." << std::endl;
			throw std::exception();
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
		std::cout << "======================" << std::endl;
		std::cout << "connect host:" << _host.c_str() << std::endl;
		std::cout << "connect user:" << _user.c_str() << std::endl;
		std::cout << "connect password:" << _password.c_str() << std::endl;
		std::cout << "connect error:" << e.what() << std::endl;
		std::cout << "======================" << std::endl;
		return NULL;
	}
	catch (std::runtime_error& e)
	{
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