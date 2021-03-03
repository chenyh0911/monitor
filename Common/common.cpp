#include "common.h"

#include "../core/db_server.h"

#define MAX_BUFFER 1024 * 16

#define current_date(cd) \
	memset(cd, '\0', sizeof(cd)); \
	time_t n = time(NULL);		\
	struct tm* t = localtime(&n); \
	snprintf(cd, sizeof(cd), "%4d%02d%02d", (t->tm_year + 1900), (t->tm_mon + 1), t->tm_mday);

#define current_time(ct) \
	memset(ct, '\0', sizeof(ct)); \
	time_t n = time(NULL);		\
	struct tm* t = localtime(&n); \
	struct timeval tv; \
	gettimeofday(&tv, NULL); \
	snprintf(ct, sizeof(ct), "%4d-%02d-%02d %02d:%02d:%02d.%03d", (t->tm_year + 1900), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, (tv.tv_usec / 1000));



int common::is_exist(const char* path)
{
	return (access(path, F_OK) == 0) ? 1 : 0;
}

int common::is_auth(const char* file, int mode)
{
	return (access(file, mode) == 0) ? 1 : 0;
}

int common::create_dir(const char* dir)
{
	if (is_exist(dir))
		return 0;

	int len;
	char path[512];
	memset(path, '\0', sizeof path);
	strncpy(path, dir, strlen(dir));
	len = strlen(path);

	for (int i = 0; i < len; i++)
	{
		if (path[i] == '/')
		{
			path[i] = '\0';
			if (access(path, 0) != 0)
			{
				if (mkdir(path, 0777) == -1)
					return -1;
			}
			path[i] = '/';
		}
	}

	if (len > 0 && access(path, 0) != 0)
	{
		if (mkdir(path, 0777) == -1)
			return -1;
	}

	return 0;
}


int common::check_date(char* _date)
{
	if (strlen(_date) == 0)
	{
		//获取当前记录日志日期
		current_date(_date);
		global::init()->clear_ip_file();
	}

	//记录日志当前日期
	char now[10];
	current_date(now);

	if (strcmp(_date, now) != 0)
	{
		//跨天写入
		global::init()->clear_ip_file();
		strcpy(_date, now);
	}

	return 0;
}


int common::file_open(FILE** pf, const char* level, const char* path)
{
	pconfig pconf = global::init()->config();
	
	if (NULL == (*pf))
	{
		//打开文件
		char log_name[128];
		memset(log_name, '\0', sizeof(log_name));
		snprintf(log_name, sizeof(log_name), "%s/%s.%s", path, pconf->_date, (pconf->f_apart) ? level : "log");
		(*pf) = fopen(log_name, "a+");
		return ((*pf) == NULL) ? errno : 0;
	}

	return 0;
}

int common::file_write(pclient client, const char* time, const char* level, const char* data)
{
	if (client->path.empty())
		return -1;

	pconfig pconf = global::init()->config();
	//检测更新日期
	check_date(pconf->_date);

	//对应句柄
	p_ip_file pif = global::init()->find_ip_file(client->ip.c_str());
	if (pif == NULL)
	{
		pif = new ip_file;
		global::init()->add_ip_file(client->ip.c_str(), pif);
	}

	FILE* pf;
	pf = (pconf->f_apart) ? global::convert_to_pf(pif, level) : pif->_log_pf;

	//打开日志
	if (file_open(&pf, level, client->path.c_str()) == 0)
	{
		char buffer[MAX_BUFFER];
		memset(buffer, '\0', sizeof buffer);
		snprintf(buffer, sizeof(buffer), "[%s:%d][%s][%s] %s\n", client->ip.c_str(), client->port, time, level, data);
	
		fwrite(buffer, strlen(buffer), 1, pf);
		fflush(pf);
	}

	return 0;
}

int common::connect_in_or_out(pclient client, int flag) 
{
	if (client->path.empty())
		return -1;

	pconfig pconf = global::init()->config();

	if (pconf->f_close == 0)
	{
		//检测更新日期
		check_date(pconf->_date);

		//对应句柄
		p_ip_file pif = global::init()->find_ip_file(client->ip.c_str());
		if (pif == NULL)
		{
			pif = new ip_file;
			global::init()->add_ip_file(client->ip.c_str(), pif);
		}

		//出入写入log文件
		FILE* pf = pif->_log_pf;

		//打开日志
		if (file_open(&pf, "log", client->path.c_str()) == 0)
		{
			char ct[32];
			current_time(ct);

			char buffer[256];
			memset(buffer, '\0', sizeof buffer);
			snprintf(buffer, sizeof(buffer), "[%s:%d][%s] %s\n", client->ip.c_str(), client->port, ct, (flag) ? "join connect" : "leave connect");

			fwrite(buffer, strlen(buffer), 1, pf);
			fflush(pf);
		}
	}

	if (pconf->d_close == 0)
	{
		char sql[128];
		memset(sql, '\0', sizeof sql);
		snprintf(sql, sizeof(sql), INSERT_EVENT, client->ip.c_str(), client->port, flag);

		if (DB::execute(sql, pconf->d_database) == -1)
		{
			std::cout << "db insert into event error:" << sql << std::endl;
		};
	}

	return 0;
}



int common::db_write(pclient client, int i_level, const char* time, const char* level, const char* data)
{
	if (client->path.empty())
		return -1;

	pconfig pconf = global::init()->config();

	char buffer[MAX_BUFFER];
	memset(buffer, '\0', sizeof buffer);
	snprintf(buffer, sizeof(buffer), INSERT_LOGGER, client->ip.c_str(), client->port, i_level, level, time, data);

	if (DB::execute(buffer, pconf->d_database) == -1)
	{
		std::cout << "db insert into logger error:" << buffer << std::endl;
	};

	return 0;
}