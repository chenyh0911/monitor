#include "db_server.h"

#include "../common/global.h"

//²éÑ¯
int DB::select(ResultSet** result, const std::string _sql, const std::string _database)
{
	Connection* conn;
	Statement* state;
	int count = 0;

	try
	{
		char db[32];
		snprintf(db, sizeof db, "use %s", _database.c_str());

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
		snprintf(db, sizeof db, "use %s", _database.c_str());

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