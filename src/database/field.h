#ifndef FIELD_H
#define FIELD_H
#include <string>

#include "database_env.h"
#include "define.h"

namespace bank::db
{
enum class field_kind : uint8
{
  none = 0,
  integer = 1,
  decimal = 2,
  string = 3,
  binary = 4,
  null = 5,
};

class field
{
  friend class result_set;

public:
  field() = default;
  ~field() = default;

  [[nodiscard]] int get_int() const;
  [[nodiscard]] double get_decimal() const;
  [[nodiscard]] std::string get_string() const;

private:
  field_kind kind_ = field_kind::none;
  sqlite3_value* value_ = nullptr;

  void set_value(sqlite3_value* new_value, uint8 type);
};
}  // namespace bank::db

#endif  // FIELD_H
