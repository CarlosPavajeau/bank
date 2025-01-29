#include <format>
#include <iomanip>
#include <iostream>

#include <sqlite3.h>

#include "db_connection.h"
#include "db_updater.h"
#include "field.h"
#include "git_revision.h"
#include "prepared_statement.h"
#include "query_result.h"

int main()
{
  std::cout << "[INFO]: git hash: " << git_revision::get_hash() << std::endl;

  bank::db::db_connection db_connection("test.db");

  if (!db_connection.open()) {
    std::cerr << "[ERROR]: failed to open database connection" << std::endl;
    return EXIT_FAILURE;
  }

  bank::db::db_updater::update(db_connection);

  db_connection.prepare_statements();

  const auto ps =
      db_connection.get_prepared_statement(bank::db::select_test_data);
  ps->set_int(0, 1);

  const auto query_result = db_connection.query(ps);

  while (query_result->next()) {
    const auto row = query_result->fetch();

    const int id = row[0].get_int();
    const auto name = row[1].get_string();

    std::cout << std::format("[INFO]: data read \n{}\n id: {}\n name: {}\n{}",
                             std::string(15, '*'),
                             id,
                             name,
                             std::string(15, '*'))
              << std::endl;
  }

  db_connection.close();

  return 0;
}
