#include <iostream>
#include <ostream>
#include <stdexcept>

#include "db_connection.h"

#include <assert.h>
#include <sqlite3/sqlite3.h>

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

sqlite3_connection::sqlite3_connection(const std::string_view db_name)
    : db_name_(db_name)
{
  db_ = nullptr;
}

sqlite3_connection::~sqlite3_connection()
{
  close();
}

bool sqlite3_connection::open()
{
  const auto open_result = sqlite3_open(db_name_.data(), &db_);
  return open_result == SQLITE_OK;
}

bool sqlite3_connection::close()
{
  assert(db_ != nullptr);

  const auto close_result = sqlite3_close(db_);
  return close_result == SQLITE_OK;
}

int sqlite3_connection::execute(const std::string_view statement)
{
  if (db_ == nullptr) {
    throw std::runtime_error(
        "sqlite3_connection::execute() called with nullptr");
  }

  char* zErrMsg = nullptr;
  const int rc = sqlite3_exec(
      db_, statement.data(), &db_connection::db_callback, nullptr, &zErrMsg);

  if (rc != SQLITE_OK) {
    std::cerr << "sql error: " << zErrMsg << std::endl;
  }

  return rc;
}

}  // namespace bank::db
