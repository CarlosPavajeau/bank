#ifndef DATABASE_ENV_H
#define DATABASE_ENV_H
#include <memory>

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;
typedef struct sqlite3_value sqlite3_value;

namespace bank::db
{
class field;

class sqlite3_prepared_statement;
class prepared_statement;

class result_set;
typedef std::shared_ptr<result_set> query_result;

class db_connection;
}  // namespace bank::db

#endif  // DATABASE_ENV_H
