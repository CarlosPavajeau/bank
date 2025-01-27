#ifndef SQLITE3_PREPARED_STATEMENT_H
#define SQLITE3_PREPARED_STATEMENT_H
#include <cstdint>
#include <string>

typedef struct sqlite3_stmt sqlite3_stmt;

namespace bank::db
{

class prepared_statement;
class db_connection;

class sqlite3_prepared_statement
{
  friend class db_connection;
  friend class prepared_statement;

public:
  sqlite3_prepared_statement(sqlite3_stmt* stmt, std::string query);
  ~sqlite3_prepared_statement();

  sqlite3_prepared_statement(const sqlite3_prepared_statement&) = delete;
  sqlite3_prepared_statement& operator=(const sqlite3_prepared_statement&) =
      delete;

  void bind_parameters(prepared_statement* stmt);

  [[nodiscard]] uint32_t get_parameter_count() const { return param_count_; }

protected:
  void set_parameter(uint8_t index, std::nullptr_t) const;
  void set_parameter(uint8_t index, bool value) const;
  void set_parameter(uint8_t index, int value) const;
  void set_parameter(uint8_t index, double value) const;
  void set_parameter(uint8_t index, std::string const& value) const;

  void clear_parameters() const;

  [[nodiscard]] sqlite3_stmt* get_stmt() const { return sqlite3_stmt_; }

  prepared_statement* stmt_;

private:
  sqlite3_stmt* sqlite3_stmt_;
  uint32_t param_count_;
  std::string query_;
};
}  // namespace bank::db

#endif  // SQLITE3_PREPARED_STATEMENT_H
