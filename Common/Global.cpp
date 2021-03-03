#include "global.h"


global* global::instance = NULL;

global::global()
	: _server(NULL), _config(NULL), _pool(NULL)
{

}


global::~global()
{

	if (instance->_server)
	{
		delete instance->_server;
		instance->_server = NULL;
	}

	if (instance->_config)
	{
		instance->clear_ip_file();
		delete instance->_config;
		instance->_config = NULL;
	}

	if (instance->_pool)
	{
		delete instance->_pool;
		instance->_pool = NULL;
	}

}


void global::add_ip_file(const std::string ip, p_ip_file pf)
{
	_config->_m_pf.insert(_ip_file_pair(ip, pf));
};

void global::del_ip_file(const std::string ip)
{
	_ip_file_map::iterator m_pf_iter = _config->_m_pf.find(ip.c_str());
	if (m_pf_iter != _config->_m_pf.end())
	{
		delete m_pf_iter->second;
		_config->_m_pf.erase(ip.c_str());
	}
};

p_ip_file global::find_ip_file(const std::string ip)
{
	_ip_file_map::iterator m_pf_iter = _config->_m_pf.find(ip.c_str());
	if (m_pf_iter == _config->_m_pf.end())
		return NULL;
	else
		return m_pf_iter->second;
};

void global::clear_ip_file() 
{
	_ip_file_map::iterator m_pf_iter = _config->_m_pf.begin();
	while (m_pf_iter != _config->_m_pf.end()) 
	{
		//ÇåÀí
		delete m_pf_iter->second;
		m_pf_iter++;
	}

	_config->_m_pf.clear();
}

int global::convert_to_level(const char* level) 
{
	if (strcmp(level, "info") == 0)
		return 1;
	else if (strcmp(level, "notice") == 0)
		return 2;
	else if (strcmp(level, "error") == 0)
		return 3;
	else
		return 0;
}


FILE* global::convert_to_pf(p_ip_file pif, const char* level)
{

	if (strcmp(level, "info") == 0)
		return pif->_info_pf;
	else if (strcmp(level, "notice") == 0)
		return pif->_notice_pf;
	else if (strcmp(level, "error") == 0)
		return pif->_error_pf;
	else
		return pif->_debug_pf;
}