#include <cassert>

#include "prepared_statement.h"

namespace bank::db
{
prepared_statement::prepared_statement(const uint32 index, const uint8 capacity)
    : index_(index)
    , data_(capacity)
{
}

void prepared_statement::set_null(const uint8 index)
{
  assert(index < data_.size());

  data_[index].data = nullptr;
}

void prepared_statement::set_bool(const uint8 index, bool value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_int8(const uint8 index, const int8 value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_int16(const uint8 index, const int16 value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_int32(const uint8 index, const int32 value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_int64(const uint8 index, const int64 value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_uint8(const uint8 index, const uint8 value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_uint16(const uint8 index, const uint16 value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_uint32(const uint8 index, const uint32 value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_uint64(const uint8 index, const uint64 value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_double(const uint8 index, const double value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_string(const uint8 index, std::string&& value)
{
  assert(index < data_.size());

  data_[index].data = std::move(value);
}

void prepared_statement::set_string(const uint8 index, std::string_view value)
{
  assert(index < data_.size());

  data_[index].data.emplace<std::string>(value);
}

}  // namespace bank::db
