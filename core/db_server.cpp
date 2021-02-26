#include "db_server.h"

#include "../common/global.h"

#define CHECK_DB "CREATE DATABASE IF NOT EXISTS %s DEFAULT CHARSET utf8 COLLATE utf8_general_ci;"
#define USE_DB "use %s;"
#define CHECK_TABLE "CREATE TABLE IF NOT EXISTS logger (" \
		"id int(11) AUTO_INCREMENT," \
		"ip varchar(16)," \
		"port int(5)," \
		"level varchar(8)," \
		"time varchar(32)," \
		"log varchar(16384)," \
		"PRIMARY KEY(`id`)" \
	") ENGINE = myisam CHARSET = utf8;"

//检测数据库是否存在
int DB::check(const std::string _database)
{
	Connection* conn;
	Statement* state;

	try
	{
		char _db[128];
		memset(_db, '\0', sizeof _db);
		snprintf(_db, sizeof _db, CHECK_DB, _database.c_str());

		char db[32];
		snprintf(db, sizeof db, USE_DB, _database.c_str());

		db_pool* pool = global::init()->pool();
		conn = pool->connect();
		state = conn->createStatement();
		state->execute(_db);
		state->execute(db);
		state->execute(CHECK_TABLE);

		delete state;
		pool->connect(conn);
	}
	catch (const std::exception& e)
	{
		std::cout << "check database error:" << e.what() << std::endl;
		return -1;
	}

	return 0;
}

//查询
int DB::select(ResultSet** result, const std::string _sql, const std::string _database)
{
	Connection* conn;
	Statement* state;
	int count = 0;

	try
	{
		char db[32];
		snprintf(db, sizeof db, USE_DB, _database.c_str());

		db_pool* pool = global::init()->pool();
		conn = pool->connect();
		state = conn->createStatement();
		state->execute(db);

		*result = state->executeQuery(_sql);
		(*result)->last();
		count = (*result)->getRow();
		(*result)->beforeFirst();

		delete state;
		pool->connect(conn);
	}
	catch (const std::exception& e)
	{
		perror(e.what());
	}

	return count;
}

int DB::execute(const std::string _sql, const std::string _database)
{
	Connection* conn;
	Statement* state;
	int count = 0;

	try
	{
		char db[32];
		snprintf(db, sizeof db, USE_DB, _database.c_str());

		db_pool* pool = global::init()->pool();
		conn = pool->connect();
		state = conn->createStatement();
		state->execute(db);

		count = state->executeUpdate(_sql);

		delete state;
		pool->connect(conn);
	}
	catch (const std::exception& e)
	{
		perror(e.what());
	}

	return count;
}