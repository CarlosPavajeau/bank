#include <cassert>
#include <iostream>
#include <ostream>
#include <stdexcept>

#include "db_connection.h"

#include <sqlite3.h>

#include "logger.h"
#include "prepared_statement.h"
#include "query_result.h"
#include "sqlite3_prepared_statement.h"

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

bool db_connection::close()
{
  assert(db_ != nullptr);

  prepared_statements_.clear();
  prepared_statement_size_.clear();

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

int db_connection::execute(prepared_statement* stmt) const
{
  assert(db_ != nullptr);

  const auto index = stmt->get_index();

  const auto sqlite3_prepared_statement = get_prepared_statement(index);
  assert(sqlite3_prepared_statement);

  sqlite3_prepared_statement->bind_parameters(stmt);

  const auto sqlite_stmt = sqlite3_prepared_statement->get_stmt();

  const auto result = sqlite3_step(sqlite_stmt);

  if (result != SQLITE_DONE) {
    LOG_ERROR("sqlite error: {}", sqlite3_errmsg(db_));
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

query_result* db_connection::query(prepared_statement* stmt) const
{
  assert(db_ != nullptr);

  const auto index = stmt->get_index();

  const auto sqlite3_prepared_statement = get_prepared_statement(index);
  assert(sqlite3_prepared_statement);

  sqlite3_prepared_statement->bind_parameters(stmt);

  const auto sqlite_stmt = sqlite3_prepared_statement->get_stmt();
  const auto column_count = sqlite3_column_count(sqlite_stmt);

  return new query_result(sqlite_stmt, column_count);
}

void db_connection::prepare_statements()
{
  prepared_statements_.resize(max_db_statements);
  prepared_statement_size_.resize(max_db_statements);

  prepare_statement(
      insert_account,
      "INSERT INTO account(username, password, email) VALUES(?, ?, ?)");

  prepare_statement(select_account,
                    "SELECT id, username, password, email, "
                    "unixepoch(created_at) FROM " "account WHERE id = ?");
}

prepared_statement* db_connection::get_prepared_statement(const uint32 index)
{
  return new prepared_statement(index, prepared_statement_size_[index]);
}

void db_connection::begin_transaction() const
{
  [[maybe_unused]] auto _ = execute("BEGIN TRANSACTION");
}

void db_connection::commit_transaction() const
{
  [[maybe_unused]] auto _ = execute("COMMIT TRANSACTION");
}

void db_connection::rollback_transaction() const
{
  [[maybe_unused]] auto _ = execute("ROLLBACK TRANSACTION");
}

sqlite3_prepared_statement* db_connection::get_prepared_statement(
    const uint32 index) const
{
  assert(index < prepared_statements_.size());

  const auto ret = prepared_statements_[index].get();

  return ret;
}

void db_connection::prepare_statement(const uint32 index,
                                      const std::string_view sql)
{
  assert(index < prepared_statements_.size());

  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, sql.data(), -1, &stmt, nullptr);
  const auto parameter_count = sqlite3_bind_parameter_count(stmt);

  prepared_statement_size_[index] = parameter_count;
  prepared_statements_[index] =
      std::make_unique<sqlite3_prepared_statement>(stmt, std::string(sql));
}

}  // namespace bank::db
