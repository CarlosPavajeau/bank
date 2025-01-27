#ifndef PREPARED_STATEMENT_H
#define PREPARED_STATEMENT_H
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace bank::db
{
struct prepared_statement_data
{
  std::variant<bool, int, double, std::string, std::nullptr_t> data;
};

class prepared_statement final
{
public:
  explicit prepared_statement(uint32_t index, uint8_t capacity);
  ~prepared_statement() = default;

  prepared_statement(const prepared_statement&) = delete;
  prepared_statement& operator=(const prepared_statement&) = delete;

  void set_null(uint8_t index);
  void set_bool(uint8_t index, bool value);
  void set_int(uint8_t index, int value);
  void set_double(uint8_t index, double value);
  void set_string(uint8_t index, std::string&& value);
  void set_string(uint8_t index, std::string_view value);

  [[nodiscard]] uint32_t get_index() const { return index_; }

  [[nodiscard]] std::vector<prepared_statement_data> const& get_parameters()
      const
  {
    return data_;
  }

protected:
  uint32_t index_;

  std::vector<prepared_statement_data> data_;
};

}  // namespace bank::db
#endif  // PREPARED_STATEMENT_H
