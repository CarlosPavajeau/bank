#include <format>
#include <fstream>

#include "db_updater.h"

#include "db_connection.h"
#include "field.h"
#include "git_revision.h"
#include "logger.h"
#include "query_result.h"
#include "update_fetcher.h"

namespace bank::db
{
bool db_updater::populate(const db_connection& connection)
{
  const auto result = retrieve(
      connection,
      R"(SELECT COUNT(*) tables FROM sqlite_master WHERE type = "table" AND name != "sqlite_sequence")");

  if (result && result->next()) {
    const auto field = result->fetch();

    if (const auto tables = field[0].get_uint64(); tables > 0) {
      return true;
    }
  }

  LOG_INFO("populating database...");

  const std::string base_file = get_base_file();
  const path base(base_file);

  if (!exists(base)) {
    LOG_INFO("base file not found");
    return false;
  }

  try {
    apply_file(connection, base);
  } catch (const std::exception&) {
    return false;
  }

  LOG_INFO("database populated");

  return true;
}

bool db_updater::update(const db_connection& connection)
{
  const path source_directory =
      std::string(git_revision::get_source_directory());

  if (!is_directory(source_directory)) {
    return false;
  }

  const update_fetcher update_fetcher(
      source_directory,
      [&](const std::string& query) { apply(connection, query); },
      [&](const std::string& query) { return retrieve(connection, query); });

  update_result result;

  try {
    result = update_fetcher.update();
  } catch (std::exception&) {
    return false;
  }

  if (result.updated == 0) {
    LOG_INFO("database is up-to date");
  } else {
    LOG_INFO("applied {} {}",
             result.updated,
             result.updated == 1 ? "query" : "queries");
  }

  return true;
}

std::string db_updater::get_base_file()
{
  return std::format(
      "{}{}", git_revision::get_source_directory(), "/sql/base/base.sql");
}

query_result db_updater::retrieve(const db_connection& connection,
                                  const std::string_view query)
{
  const auto result = connection.query(query);

  return result;
}

void db_updater::apply(const db_connection& connection,
                       const std::string_view query)
{
  [[maybe_unused]] auto _ = connection.execute(query);
}

void db_updater::apply_file(const db_connection& connection, const path& path)
{
  std::ifstream file(path.generic_string());

  if (!file.is_open()) {
    throw std::runtime_error("file could not be openend");
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  const auto query = buffer.str();

  apply(connection, query);
}

}  // namespace bank::db
