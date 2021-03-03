#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "server.h"
#include "../core/db_pool.h"

#include <map>

#pragma region ≈‰÷√œÓƒ¨»œ÷µ

#define SERVER_PORT 8000
#define SERVER_MAX_CLIENT 5
#define SERVER_MAX_THREAD 10
#define SERVER_MAX_TASK 20

#define CMD_CLOSE 0
#define CMD_LEVEL 0

#define FILE_CLOSE 0
#define FILE_LEVEL 0
#define FILE_APART 0
#define FILE_DIR "./logs"

#define DB_CLOSE 1
#define DB_LEVEL 0
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PORT "3306"
#define DB_PASSWORD "123456"
#define DB_DATABASE "logger"
#define DB_MIN_POOL_SIZE 5
#define DB_MAX_POOL_SIZE 10

#pragma endregion

typedef struct _ip_file {

	char _ip[16];
	FILE* _log_pf;
	FILE* _debug_pf;
	FILE* _info_pf;
	FILE* _notice_pf;
	FILE* _error_pf;

	_ip_file()
	{
		memset(_ip, '\0', sizeof _ip);
		_log_pf = NULL;
		_debug_pf = NULL;
		_info_pf = NULL;
		_notice_pf = NULL;
		_error_pf = NULL;
	}

	~_ip_file() 
	{
		memset(_ip, '\0', sizeof _ip);
		if (_log_pf != NULL)
		{
			fclose(_log_pf);
			_log_pf = NULL;
		}
		if (_debug_pf != NULL) 
		{
			fclose(_debug_pf);
			_debug_pf = NULL;
		}
		if (_info_pf != NULL)
		{
			fclose(_info_pf);
			_info_pf = NULL;
		}
		if (_notice_pf != NULL)
		{
			fclose(_notice_pf);
			_notice_pf = NULL;
		}
		if (_error_pf != NULL)
		{
			fclose(_error_pf);
			_error_pf = NULL;
		}
	}

} ip_file, *p_ip_file;

typedef std::map<std::string, p_ip_file> _ip_file_map;
typedef std::pair<std::string, p_ip_file> _ip_file_pair;

typedef struct _CONFIG 
{
	int s_port;
	int	s_max_client;
	int s_max_thread;
	int s_max_task;

	int c_close;
	int c_level;

	int f_close;
	int f_level;
	int f_apart;
	std::string f_dir;

	int d_close;
	int d_level;
	std::string d_host;
	std::string d_user;
	std::string d_port;
	std::string d_password;
	std::string d_database;
	int d_min_pool_size;
	int d_max_pool_size;

	_ip_file_map _m_pf;
	char _date[10];

	_CONFIG() 
	{
		s_port = SERVER_PORT;
		s_max_client = SERVER_MAX_CLIENT;
		s_max_thread = SERVER_MAX_THREAD;
		s_max_task = SERVER_MAX_TASK;

		c_close = CMD_CLOSE;
		c_level = CMD_LEVEL;

		f_close = FILE_CLOSE;
		f_level = FILE_LEVEL;
		f_apart = FILE_APART;
		f_dir = FILE_DIR;

		d_close = DB_CLOSE;
		d_level = DB_LEVEL;
		d_host = DB_HOST;
		d_user = DB_USER;
		d_port = DB_PORT;
		d_password = DB_PASSWORD;
		d_database = DB_DATABASE;
		d_min_pool_size = DB_MIN_POOL_SIZE;
		d_max_pool_size = DB_MAX_POOL_SIZE;

		//memset(_date, '\0', sizeof _date);
	}

} config, *pconfig;



class global
{
public:
	global();
	virtual ~global();

public:

	void socket(server* socket) { _server = socket; };
	server* socket() { return _server; };

	void config(pconfig& conf) { _config = conf; };
	pconfig config() { return _config; };

	void pool(db_pool* pool) { _pool = pool; };
	db_pool* pool() { return _pool; };

public:

	void add_ip_file(const std::string ip, p_ip_file pf);
	void del_ip_file(const std::string ip);
	p_ip_file find_ip_file(const std::string ip);
	void clear_ip_file();

	static int convert_to_level(const char* level);
	static FILE* convert_to_pf(p_ip_file pif, const char* level);

public:

	static global* init()
	{
		if (!instance)
			instance = new global;

		return instance;
	};

	static void release() 
	{
		if (instance)
			delete instance;
	};

private:
	static global* instance;

	server* _server;
	pconfig _config;
	db_pool* _pool;

};


#endif // !_GLOBAL_H_



