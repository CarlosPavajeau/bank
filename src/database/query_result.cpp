#include "query_result.h"

#include <sqlite3.h>

#include "field.h"

namespace bank::db
{
result_set::result_set(sqlite3_stmt* stmt, const uint32 column_count)
    : stmt_(stmt)
    , column_count_(column_count)
{
  current_ = new field[column_count_];
}

result_set::~result_set()
{
  clean();
}

bool result_set::next()
{
  if (!stmt_) {
    return false;
  }

  if (const auto result = sqlite3_step(stmt_); result != SQLITE_ROW) {
    clean();
    return false;
  }

  if (const auto column_count = sqlite3_column_count(stmt_);
      column_count != column_count_)
  {
    clean();
    return false;
  }

  for (int i = 0; i < column_count_; ++i) {
    const auto column_type = sqlite3_column_type(stmt_, i);
    const auto column_value = sqlite3_column_value(stmt_, i);

    current_[i].set_value(column_value, column_type);
  }

  return true;
}

void result_set::clean()
{
  if (current_) {
    delete[] current_;
    current_ = nullptr;
  }

  if (stmt_) {
    sqlite3_reset(stmt_);
    stmt_ = nullptr;
  }
}

}  // namespace bank::db
