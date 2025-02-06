#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include "database_env.h"
#include "define.h"

namespace bank::db
{
class result_set
{
public:
  result_set(sqlite3_stmt* stmt, uint32 column_count);
  ~result_set();

  result_set(const result_set&) = delete;
  result_set& operator=(const result_set&) = delete;

  [[nodiscard]] field* fetch() const { return current_; }

  [[nodiscard]] bool next();

private:
  sqlite3_stmt* stmt_ = nullptr;
  uint32 column_count_ = 0;

  field* current_ = nullptr;

  void clean();
};
}  // namespace bank::db

#endif  // QUERY_RESULT_H
