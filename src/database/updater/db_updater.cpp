#include <format>
#include <iostream>

#include "db_updater.h"

#include "db_connection.h"
#include "git_revision.h"
#include "query_result.h"
#include "update_fetcher.h"

namespace bank::db
{
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
    std::cout << "[INFO]: database is up-to date" << std::endl;
  } else {
    std::cout << std::format("[INFO]: applied {} {}",
                             result.updated,
                             result.updated == 1 ? "query" : "queries")
              << std::endl;
  }

  return true;
}

db_updater::query_result_ptr db_updater::retrieve(
    const db_connection& connection, const std::string_view query)
{
  const auto result = connection.query(query);

  return result;
}

void db_updater::apply(const db_connection& connection,
                       const std::string_view query)
{
  [[maybe_unused]] auto _ = connection.execute(query);
}

}  // namespace bank::db
