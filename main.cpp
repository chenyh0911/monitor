#include <cstdio>

#include "./util/ini.h"
#include "./common/global.h"
#include "./worker/epoll_server.h"
#include "./core/db_server.h"

#define ISNOT_EMPTY(s) if(s.compare("") != 0)
#define CONFIG_PATH "/projects/monitor/config.ini"

using namespace DB;

int convert_to_int(std::string str) 
{
    std::stringstream ss;
    ss << str;

    int i;
    ss >> i;
    return i;
}

int load_config(mnodes nodes) 
{
    std::string s_port = ini::get_value(nodes, "SERVER", "s_port");
    std::string s_max_client = ini::get_value(nodes, "SERVER", "s_max_client");
    std::string s_max_thread = ini::get_value(nodes, "SERVER", "s_max_thread");
    std::string s_max_task = ini::get_value(nodes, "SERVER", "s_max_task");

    std::string c_close = ini::get_value(nodes, "CMD", "c_close");
    std::string c_level = ini::get_value(nodes, "CMD", "c_level");

    std::string f_close = ini::get_value(nodes, "FILE", "f_close");
    std::string f_level = ini::get_value(nodes, "FILE", "f_level");
    std::string f_apart = ini::get_value(nodes, "FILE", "f_apart");
    std::string f_dir = ini::get_value(nodes, "FILE", "f_dir");

    std::string d_close = ini::get_value(nodes, "DB", "d_close");
    std::string d_level = ini::get_value(nodes, "DB", "d_level");
    std::string d_apart = ini::get_value(nodes, "DB", "d_apart");
    std::string d_host = ini::get_value(nodes, "DB", "d_host");
    std::string d_user = ini::get_value(nodes, "DB", "d_user");
    std::string d_port = ini::get_value(nodes, "DB", "d_port");
    std::string d_password = ini::get_value(nodes, "DB", "d_password");
    std::string d_database = ini::get_value(nodes, "DB", "d_database");
    std::string d_min_pool_size = ini::get_value(nodes, "DB", "d_min_pool_size");
    std::string d_max_pool_size = ini::get_value(nodes, "DB", "d_max_pool_size");

    pconfig _config = new config;

	ISNOT_EMPTY(s_port)
		_config->s_port = convert_to_int(s_port);
	ISNOT_EMPTY(s_max_client)
		_config->s_max_client = convert_to_int(s_max_client);
	ISNOT_EMPTY(s_max_thread)
		_config->s_max_thread = convert_to_int(s_max_thread);
    ISNOT_EMPTY(s_max_task)
        _config->s_max_task = convert_to_int(s_max_task);

    ISNOT_EMPTY(c_close)
        _config->c_close = convert_to_int(c_close);
    ISNOT_EMPTY(c_level)
        _config->c_level = convert_to_int(c_level);

    ISNOT_EMPTY(f_close)
        _config->f_close = convert_to_int(f_close);
    ISNOT_EMPTY(f_level)
        _config->f_level = convert_to_int(f_level);
    ISNOT_EMPTY(f_apart)
        _config->f_apart = convert_to_int(f_apart);
    ISNOT_EMPTY(f_dir)
        _config->f_dir = f_dir.c_str();

    ISNOT_EMPTY(d_close)
        _config->d_close = convert_to_int(d_close);
    ISNOT_EMPTY(d_level)
        _config->d_level = convert_to_int(d_level);
    ISNOT_EMPTY(d_apart)
        _config->d_apart = convert_to_int(d_apart);
    ISNOT_EMPTY(d_host)
        _config->d_host = d_host.c_str();
    ISNOT_EMPTY(d_user)
        _config->d_user = d_user.c_str();
    ISNOT_EMPTY(d_port)
        _config->d_port = d_port.c_str();
    ISNOT_EMPTY(d_password)
        _config->d_password = d_password.c_str();
    ISNOT_EMPTY(d_database)
        _config->d_database = d_database.c_str();
    ISNOT_EMPTY(d_min_pool_size)
        _config->d_min_pool_size = convert_to_int(d_min_pool_size);
    ISNOT_EMPTY(d_max_pool_size)
        _config->d_max_pool_size = convert_to_int(d_max_pool_size);

    global::init()->config(_config);
    return 0;
}

int check_config() 
{
    pconfig _config = global::init()->config();
    //conf->s_port 
}

int main()
{
    server* _server = NULL;
    db_pool* _pool = NULL;
    thread_pool<base_task>* pool = NULL;
    epoll_server* epoll = NULL;

    global::init();

    //加载配置信息
    mnodes nodes;
    ini::read(CONFIG_PATH, nodes);
    load_config(nodes);

    pconfig _config = global::init()->config();

    //开启server
    _server = new server(_config->s_port, _config->s_max_client);
    if (_server->init() == -1)
    {
        goto _QUIT;
    };
    _server->run();
    global::init()->socket(_server);

    //开启数据库连接池
    _pool = new db_pool(_config->d_host, _config->d_user, _config->d_port, _config->d_password, _config->d_min_pool_size, _config->d_max_pool_size);
    global::init()->pool(_pool);

    //检测数据
    if (DB::check(_config->d_database) == -1)
    {
        goto _QUIT;
    };

    //开启线程池
    pool = new thread_pool<base_task>(_config->s_max_thread, _config->s_max_task);
    if (pool->init() == -1)
    {
        goto _QUIT;
    };

    //开启管道
    epoll = new epoll_server(_server, pool);
    if (epoll->init() == -1)
    {
        goto _QUIT;
    };
    epoll->run();

_QUIT:
	global::release();
    return 0;
}