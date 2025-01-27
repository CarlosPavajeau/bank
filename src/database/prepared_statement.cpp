#include <cassert>

#include "prepared_statement.h"

namespace bank::db
{
prepared_statement::prepared_statement(const uint32_t index,
                                       const uint8_t capacity)
    : index_(index)
    , data_(capacity)
{
}

void prepared_statement::set_null(const uint8_t index)
{
  assert(index < data_.size());

  data_[index].data = nullptr;
}

void prepared_statement::set_bool(const uint8_t index, bool value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_int(const uint8_t index, const int value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_double(const uint8_t index, const double value)
{
  assert(index < data_.size());

  data_[index].data = value;
}

void prepared_statement::set_string(const uint8_t index, std::string&& value)
{
  assert(index < data_.size());

  data_[index].data = std::move(value);
}

void prepared_statement::set_string(const uint8_t index, std::string_view value)
{
  assert(index < data_.size());

  data_[index].data.emplace<std::string>(value);
}

}  // namespace bank::db
