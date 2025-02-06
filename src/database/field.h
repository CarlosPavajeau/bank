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

  [[nodiscard]] int8 get_int8() const;
  [[nodiscard]] int16 get_int16() const;
  [[nodiscard]] int32 get_int32() const;
  [[nodiscard]] int64 get_int64() const;

  [[nodiscard]] uint8 get_uint8() const;
  [[nodiscard]] uint16 get_uint16() const;
  [[nodiscard]] uint32 get_uint32() const;
  [[nodiscard]] uint64 get_uint64() const;

  [[nodiscard]] double get_decimal() const;
  [[nodiscard]] std::string get_string() const;

private:
  field_kind kind_ = field_kind::none;
  sqlite3_value* value_ = nullptr;

  void set_value(sqlite3_value* new_value, uint8 type);
};
}  // namespace bank::db

#endif  // FIELD_H
