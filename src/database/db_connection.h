#ifndef CONNECTION_H
#define CONNECTION_H
#include <memory>
#include <string_view>
#include <vector>

#include "define.h"

typedef struct sqlite3 sqlite3;

namespace bank::db
{

class query_result;
class sqlite3_prepared_statement;
class prepared_statement;

enum db_statements : uint32
{
  select_test_data,

  max_db_statements
};

class db_connection
{
public:
  explicit db_connection(std::string_view db_name);
  ~db_connection();

  bool open();
  bool close();

  [[nodiscard]] int execute(std::string_view statement) const;
  [[nodiscard]] query_result* query(std::string_view statement) const;
  [[nodiscard]] query_result* query(prepared_statement* stmt) const;

  void prepare_statements();

  prepared_statement* get_prepared_statement(uint32 index);

protected:
  [[nodiscard]] sqlite3_prepared_statement* get_prepared_statement(
      uint32 index) const;
  void prepare_statement(uint32 index, std::string_view sql);

  typedef std::vector<std::unique_ptr<sqlite3_prepared_statement>>
      prepared_statement_container;
  prepared_statement_container prepared_statements_;

private:
  sqlite3* db_;
  std::string_view db_name_;

  std::vector<uint32> prepared_statement_size_;

  static int db_callback(void* data, int argc, char** argv, char** azColName);
};
}  // namespace bank::db

#endif  // CONNECTION_H
