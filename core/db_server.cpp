#include "db_server.h"

#include "../common/global.h"

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
		state->execute(CHECK_LOGGER_TABLE);
		state->execute(CHECK_EVENT_TABLE);

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
		count = -1;
	}

	return count;
}