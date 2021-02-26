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
		delete instance->_config;
		instance->_config = NULL;
	}

	if (instance->_pool)
	{
		delete instance->_pool;
		instance->_pool = NULL;
	}

}
