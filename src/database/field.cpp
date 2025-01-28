#include <stdexcept>

#include "field.h"

#include "sqlite3.h"

namespace bank::db
{
int field::get_int() const
{
  if (kind_ != field_kind::integer) {
    throw std::invalid_argument("field::get_int: bad field kind");
  }

  if (!value_) {
    return 0;
  }

  const auto result = sqlite3_value_int(value_);
  return result;
}

double field::get_decimal() const
{
  if (kind_ != field_kind::decimal) {
    throw std::invalid_argument("field::get_decimal: bad field kind");
  }

  if (!value_) {
    return 0.0;
  }

  const auto result = sqlite3_value_double(value_);
  return result;
}

std::string field::get_string() const
{
  if (kind_ != field_kind::string) {
    throw std::invalid_argument("field::get_string: bad field kind");
  }

  if (!value_) {
    return {};
  }

  const auto result = sqlite3_value_text(value_);

  return reinterpret_cast<const char*>(result);
}

void field::set_value(sqlite3_value* new_value, const uint8 type)
{
  value_ = new_value;
  kind_ = static_cast<field_kind>(type);
}

}  // namespace bank::db
