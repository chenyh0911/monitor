#include "logger.h"


int logger::parse(const std::string data, std::string& level, std::string& time, std::string& log)
{
	return 0;
}

int logger::scheduler(const std::string ip, int port, const std::string data)
{
	//���ݷ���
	std::cout << "ip:" << ip << std::endl;
	std::cout << "port:" << port << std::endl;
	std::cout << "data:" << data << std::endl;

	std::string level;
	std::string time;
	std::string log;

	//������־
	if (parse(data, level, time, log) == 0)
	{

	};

	return 0;
}

