#ifndef DB_UPDATER_H
#define DB_UPDATER_H
#include <filesystem>

#include "database_env.h"

namespace bank::db
{

class db_updater
{
public:
  static bool populate(const db_connection& connection);
  static bool update(const db_connection& connection);

private:
  typedef std::filesystem::path path;

  static std::string get_base_file();

  static query_result retrieve(const db_connection& connection,
                               std::string_view query);
  static void apply(const db_connection& connection, std::string_view query);
  static void apply_file(const db_connection& connection, const path& path);
};
}  // namespace bank::db

#endif  // DB_UPDATER_H
