#include <cassert>
#include <iostream>
#include <ostream>
#include <stdexcept>

#include "db_connection.h"

#include <sqlite3.h>

#include "query_result.h"

namespace bank::db
{
int db_connection::db_callback(void* /* data */,
                               const int argc,
                               char** argv,
                               char** azColName)
{
  printf("sqlite callback\n");
  for (int i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

db_connection::db_connection(const std::string_view db_name)
    : db_name_(db_name)
{
  db_ = nullptr;
}

db_connection::~db_connection()
{
  [[maybe_unused]] auto _ = close();
}

bool db_connection::open()
{
  const auto open_result = sqlite3_open(db_name_.data(), &db_);
  return open_result == SQLITE_OK;
}

bool db_connection::close() const
{
  assert(db_ != nullptr);

  const auto close_result = sqlite3_close(db_);
  return close_result == SQLITE_OK;
}

int db_connection::execute(const std::string_view statement) const
{
  assert(db_ != nullptr);

  char* error_message = nullptr;
  const int result = sqlite3_exec(db_,
                                  statement.data(),
                                  &db_connection::db_callback,
                                  nullptr,
                                  &error_message);

  if (result != SQLITE_OK) {
    std::cerr << "sql error: " << error_message << std::endl;
  }

  return result;
}

query_result* db_connection::query(const std::string_view statement) const
{
  assert(db_ != nullptr);

  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, statement.data(), -1, &stmt, nullptr);

  const auto column_count = sqlite3_column_count(stmt);

  return new query_result(stmt, column_count);
}

}  // namespace bank::db
