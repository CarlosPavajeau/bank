#ifndef CONNECTION_H
#define CONNECTION_H
#include <string_view>

typedef struct sqlite3 sqlite3;

namespace bank::db
{
class db_connection
{
public:
  db_connection() = default;
  virtual ~db_connection() = default;

  virtual bool open() = 0;
  virtual bool close() = 0;

  virtual int execute(std::string_view statement) = 0;

  static int db_callback(void* data, int argc, char** argv, char** azColName);
};

class sqlite3_connection final : public db_connection
{
public:
  explicit sqlite3_connection(std::string_view db_name);
  ~sqlite3_connection() override;

  bool open() override;
  bool close() override;

  int execute(std::string_view statement) override;

private:
  sqlite3* db_;
  std::string_view db_name_;
};
}  // namespace bank::db

#endif  // CONNECTION_H
