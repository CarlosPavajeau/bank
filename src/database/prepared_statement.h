#ifndef PREPARED_STATEMENT_H
#define PREPARED_STATEMENT_H
#include <string>
#include <variant>
#include <vector>

#include "define.h"

namespace bank::db
{
struct prepared_statement_data
{
  std::variant<bool,
               int8,
               int16,
               int32,
               int64,
               uint8,
               uint16,
               uint32,
               uint64,
               double,
               std::string,
               std::nullptr_t>
      data;
};

class prepared_statement final
{
public:
  explicit prepared_statement(uint32 index, uint8 capacity);
  ~prepared_statement() = default;

  prepared_statement(const prepared_statement&) = delete;
  prepared_statement& operator=(const prepared_statement&) = delete;

  void set_null(uint8 index);
  void set_bool(uint8 index, bool value);

  void set_int8(uint8 index, int8 value);
  void set_int16(uint8 index, int16 value);
  void set_int32(uint8 index, int32 value);
  void set_int64(uint8 index, int64 value);

  void set_uint8(uint8 index, uint8 value);
  void set_uint16(uint8 index, uint16 value);
  void set_uint32(uint8 index, uint32 value);
  void set_uint64(uint8 index, uint64 value);

  void set_double(uint8 index, double value);

  void set_string(uint8 index, std::string&& value);
  void set_string(uint8 index, std::string_view value);

  [[nodiscard]] uint32 get_index() const { return index_; }

  [[nodiscard]] std::vector<prepared_statement_data> const& get_parameters()
      const
  {
    return data_;
  }

protected:
  uint32 index_;

  std::vector<prepared_statement_data> data_;
};

}  // namespace bank::db
#endif  // PREPARED_STATEMENT_H
