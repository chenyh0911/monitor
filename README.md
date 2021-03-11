
monitor —— logger 日志监控
===============

> V1.0.3

## 简介

V1.0.3 支持

 + linux monitor
 + linux php_logger
 + windows monitor
 + windows php_logger
 
## 功能

 + 支持跨平台 windows/linux
 + 支持配置热更新
 + 支持socket断线重连重传
 + 支持内存共享
 + 支持日志文本分类存储
 + 支持日志自动存储数据库

## 使用

> linux monitor

~~~
 + l_monitor.out 运行程序，项目提供 linux下使用
 + pthread库 系统自带 
 + mysqlcppconn库 可以使用本项目提供 如果不开启数据库存储日志，则不需要使用
 + ssl	系统自带,用于支持mysqlcppconn
 + crypto 系统自带,用于支持mysqlcppconn
 + boost_regex 网络下载
 + config.ini 项目提供，可自行修改 自行配置路径运行命令 -c xxx/xxx.ini，不使用则默认加载 根目录下config.ini文件  
~~~

> linux php_logger

~~~
 + logger.so PHP扩展库，项目提供 linux下使用
 + php-7.2.x ZTS版 网络下载自己编译 
 + pthread库 系统自带
~~~

> windows monitor

~~~
 + w_monitor.exe 运行程序，项目提供 windows下使用
 + 所需依赖完整上传，项目提供
 + 登录界面配置信息，可开关数据库支持
 + 界面支持监控显示单一进程日志数据，或所有进程数据
~~~

> windows php_logger

~~~
 + php_logger.dll PHP扩展库，项目提供 windows下使用
 + php-7.2.x ZTS版 网络下载自己编译 
 + pthread库 项目提供，可网络下载 pthreadVC2.dll置于php.exe同级目录
~~~

> monitor使用配置

~~~
config.ini中添加

[SERVER]
s_port=8000
s_max_client=10
s_max_thread=5

[CMD]
c_close=0
c_level=0

[FILE]
f_close=0
f_level=0
f_apart=0 
f_dir=./logs

[DB]
d_close=1
d_level=0
d_host=192.168.1.250
d_user=root
d_port=3306
d_password=123456
d_database=logger
d_min_pool_size=5
d_max_pool_size=10
~~~

> php_logger使用配置

~~~
php.ini中添加
extension=logger
[logger]
logger.close=0
logger.level=0
logger.level_strict=0
logger.method=2
logger.file_dir=./logs
logger.server_ip=192.168.1.201 
logger.server_port=8000
logger.server_retime=3
~~~

## 版权信息

 + @auther yh chen 
 + @version 1.0.3
 + @date 2021-03-04 11:11
