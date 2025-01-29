#ifndef DB_UPDATER_H
#define DB_UPDATER_H
#include <filesystem>

namespace bank::db
{

class db_connection;
class query_result;

class db_updater
{
public:
  static bool update(const db_connection& connection);

private:
  typedef query_result* query_result_ptr;
  typedef std::filesystem::path path;

  static query_result_ptr retrieve(const db_connection& connection,
                                   std::string_view query);
  static void apply(const db_connection& connection, std::string_view query);
};
}  // namespace bank::db

#endif  // DB_UPDATER_H
