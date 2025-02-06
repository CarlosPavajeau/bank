#include <stdexcept>

#include "field.h"

#include "sqlite3.h"

namespace bank::db
{
int8 field::get_int8() const
{
  if (kind_ != field_kind::integer || value_ == nullptr) {
    throw std::invalid_argument("field::get_int8: bad field kind");
  }

  const auto result = sqlite3_value_int(value_);
  return static_cast<int8>(result);
}

int16 field::get_int16() const
{
  if (kind_ != field_kind::integer || value_ == nullptr) {
    throw std::invalid_argument("field::get_int16: bad field kind");
  }

  const auto result = sqlite3_value_int(value_);
  return static_cast<int16>(result);
}

int32 field::get_int32() const
{
  if (kind_ != field_kind::integer || value_ == nullptr) {
    throw std::invalid_argument("field::get_int: bad field kind");
  }

  const auto result = sqlite3_value_int(value_);
  return result;
}

int64 field::get_int64() const
{
  if (kind_ != field_kind::integer || value_ == nullptr) {
    throw std::invalid_argument("field::get_int: bad field kind");
  }

  const auto result = sqlite3_value_int64(value_);

  return result;
}

uint8 field::get_uint8() const
{
  if (kind_ != field_kind::integer || value_ == nullptr) {
    throw std::invalid_argument("field::get_int: bad field kind");
  }

  const auto result = sqlite3_value_int(value_);

  return result;
}

uint16 field::get_uint16() const
{
  if (kind_ != field_kind::integer || value_ == nullptr) {
    throw std::invalid_argument("field::get_int: bad field kind");
  }

  const auto result = sqlite3_value_int(value_);

  return result;
}

uint32 field::get_uint32() const
{
  if (kind_ != field_kind::integer || value_ == nullptr) {
    throw std::invalid_argument("field::get_int: bad field kind");
  }

  const auto result = sqlite3_value_int64(value_);

  return result;
}

uint64 field::get_uint64() const
{
  if (kind_ != field_kind::integer || value_ == nullptr) {
    throw std::invalid_argument("field::get_int: bad field kind");
  }

  const auto result = sqlite3_value_int64(value_);

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
