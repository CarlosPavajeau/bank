#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include "define.h"

typedef struct sqlite3_stmt sqlite3_stmt;

namespace bank::db
{

class field;

class query_result
{
public:
  query_result(sqlite3_stmt* stmt, uint32 column_count);
  ~query_result();

  query_result(const query_result&) = delete;
  query_result& operator=(const query_result&) = delete;

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
