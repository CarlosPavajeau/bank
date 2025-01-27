#ifndef CONNECTION_H
#define CONNECTION_H
#include <string_view>

typedef struct sqlite3 sqlite3;

namespace bank::db
{

class query_result;

class db_connection
{
public:
  explicit db_connection(std::string_view db_name);
  ~db_connection();

  bool open();
  [[nodiscard]] bool close() const;
  [[nodiscard]] int execute(std::string_view statement) const;
  [[nodiscard]] query_result* query(std::string_view statement) const;

private:
  sqlite3* db_;
  std::string_view db_name_;

  static int db_callback(void* data, int argc, char** argv, char** azColName);
};
}  // namespace bank::db

#endif  // CONNECTION_H
