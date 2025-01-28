#include <cassert>

#include "sqlite3_prepared_statement.h"

#include <sqlite3.h>

#include "prepared_statement.h"

namespace bank::db
{
sqlite3_prepared_statement::sqlite3_prepared_statement(sqlite3_stmt* stmt,
                                                       std::string query)
    : stmt_(nullptr)
    , sqlite3_stmt_(stmt)
    , query_(std::move(query))
{
  param_count_ = sqlite3_bind_parameter_count(stmt);
}

sqlite3_prepared_statement::~sqlite3_prepared_statement()
{
  clear_parameters();

  sqlite3_finalize(sqlite3_stmt_);
}

void sqlite3_prepared_statement::bind_parameters(prepared_statement* stmt)
{
  stmt_ = stmt;

  uint8 pos = 1;

  for (const auto& [data] : stmt->get_parameters()) {
    std::visit([&](auto&& param) { set_parameter(pos, param); }, data);

    ++pos;
  }
}

void sqlite3_prepared_statement::clear_parameters() const
{
  sqlite3_clear_bindings(sqlite3_stmt_);
}

void sqlite3_prepared_statement::set_parameter(const uint8 index,
                                               std::nullptr_t) const
{
  assert(index <= param_count_);

  sqlite3_bind_null(sqlite3_stmt_, index);
}

void sqlite3_prepared_statement::set_parameter(const uint8 index,
                                               const bool value) const
{
  assert(index <= param_count_);

  sqlite3_bind_int(sqlite3_stmt_, index, value);
}

void sqlite3_prepared_statement::set_parameter(const uint8 index,
                                               const int value) const
{
  assert(index <= param_count_);

  sqlite3_bind_int(sqlite3_stmt_, index, value);
}

void sqlite3_prepared_statement::set_parameter(const uint8 index,
                                               const double value) const
{
  assert(index <= param_count_);

  sqlite3_bind_double(sqlite3_stmt_, index, value);
}

void sqlite3_prepared_statement::set_parameter(const uint8 index,
                                               std::string const& value) const
{
  assert(index <= param_count_);

  sqlite3_bind_text(sqlite3_stmt_,
                    index,
                    value.data(),
                    static_cast<int>(value.size()),
                    SQLITE_STATIC);
}

}  // namespace bank::db
