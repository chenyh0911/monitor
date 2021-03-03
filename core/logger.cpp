#include "logger.h"

#include <boost/regex.hpp>

#include "../common/common.h"

#define REGEX_EXP "(\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]{3}] \\[[a-z]{1,9}])"


int parse_head(const std::string head, char* time, char* level)
{

	std::string::size_type time_pos_s = std::string::npos;
	std::string::size_type time_pos_e = std::string::npos;
	std::string::size_type level_pos_s = std::string::npos;
	std::string::size_type level_pos_e = std::string::npos;

	//查找时间坐标
	time_pos_s = head.find("[");
	time_pos_e = head.find("]");
	if (time_pos_s != std::string::npos && time_pos_e != std::string::npos
		&& time_pos_s < time_pos_e)
		memcpy(time, head.substr(time_pos_s + 1, time_pos_e - time_pos_s - 1).c_str(), time_pos_e - time_pos_s - 1);
	else
		return -1;

	//查找等级坐标
	level_pos_s = head.find("[", time_pos_e + 1);
	level_pos_e = head.find("]", time_pos_e + 1);
	if (level_pos_s != std::string::npos && level_pos_e != std::string::npos
		&& level_pos_s < level_pos_e)
		memcpy(level, head.substr(level_pos_s + 1, level_pos_e - level_pos_s - 1).c_str(), level_pos_e - level_pos_s - 1);
	else
		return -1;

	return 0;
}

int parse_tail(const std::string text, boost::regex pattern, char* data)
{
	boost::sregex_iterator iter(text.begin(), text.end(), pattern);
	boost::sregex_iterator end;
	if (iter == end)
		return -1;

	memcpy(data, iter->prefix().str().c_str(), iter->prefix().length());
	return 0;
}

int logger::parse(const std::string buffer, std::vector<plog>& logs)
{
	boost::regex pattern(REGEX_EXP, boost::regex::icase); //忽略大小写
	boost::sregex_iterator iter(buffer.begin(), buffer.end(), pattern);
	boost::sregex_iterator end;
	if (iter == end)
		return -1;

	for (; iter != end; *iter++)
	{
		llog l;
		if (parse_head(iter->str().c_str(), l.time, l.level))
			continue;

		if (parse_tail(iter->suffix().str().c_str(), pattern, l.data))
			memcpy(l.data, iter->suffix().str().c_str(), iter->suffix().length());

		logs.push_back(&l);
	}

	return 0;
}

int logger::scheduler(int _cfd, const std::string buffer)
{
	//数据分析
	std::vector<plog> logs;

	//解析日志
	if (parse(buffer, logs)
		|| buffer.empty())
		return -1;

	std::cout << "================================" << std::endl;

	//配置
	pconfig _config = global::init()->config();
	//客户端
	pclient client = global::init()->socket()->find_client(_cfd);

	for (std::vector<plog>::iterator iter = logs.begin(); iter != logs.end(); iter++)
	{
		int i_level = global::convert_to_level((*iter)->level);

		std::string time = (*iter)->time;
		std::string level = (*iter)->level;
		std::string data = (*iter)->data;
		if (_config->c_close == 0)
		{
			//控制台打印日志
			if (_config->c_level <= i_level)
			{
				std::cout << "ip:" << client->ip << std::endl;
				std::cout << "port:" << client->port << std::endl;
				//std::cout << "buffer:" << buffer << std::endl;
				//std::cout << "size:" << buffer.size() << std::endl;
				std::cout << "time:" << time << std::endl;
				std::cout << "level:" << level << std::endl;
				std::cout << "data:" << data << std::endl;
			}
		}

		if (_config->f_close == 0)
		{
			//写入日志文本
			if (_config->f_level <= i_level)
			{
				common::file_write(client, time.c_str(), level.c_str(), data.c_str());
			}
			
		}

		if (_config->d_close == 0)
		{
			//写入数据库
			if (_config->f_level <= i_level)
			{
				common::db_write(client, i_level, time.c_str(), level.c_str(), data.c_str());
			}
		}
	}

	std::cout << "================================" << std::endl;
	return 0;
}
