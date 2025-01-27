#include <iomanip>
#include <iostream>

#include <sqlite3.h>

#include "db_connection.h"
#include "field.h"
#include "git_revision.h"
#include "prepared_statement.h"
#include "query_result.h"

int main()
{
  bank::db::db_connection db_connection("test.db");

  std::cout << "git hash: " << git_revision::get_hash() << std::endl;

  if (!db_connection.open()) {
    std::cerr << "Failed to open database connection" << std::endl;
    return EXIT_FAILURE;
  }

  if (const int create_table_result = db_connection.execute(
          "CREATE TABLE IF NOT EXISTS test(id int, name VARCHAR(32))");
      create_table_result != SQLITE_OK)
  {
    db_connection.close();
    return EXIT_FAILURE;
  }

  if (const int delete_all_result = db_connection.execute("DELETE FROM test");
      delete_all_result != SQLITE_OK)
  {
    db_connection.close();
    return EXIT_FAILURE;
  }

  if (const int insert_result = db_connection.execute(
          R"(INSERT INTO test VALUES (1, "Namono"), (2, "Pedro"))");
      insert_result != SQLITE_OK)
  {
    db_connection.close();
    return EXIT_FAILURE;
  }

  db_connection.prepare_statements();

  const auto ps =
      db_connection.get_prepared_statement(bank::db::select_test_data);
  ps->set_int(0, 1);

  const auto query_result = db_connection.query(ps);

  while (query_result->next()) {
    const auto row = query_result->fetch();

    const int id = row[0].get_int();
    const auto name = row[1].get_string();

    std::cout << std::string(15, '*') << std::endl;
    std::cout << "data read" << std::endl;
    std::cout << "id: " << id << std::endl;
    std::cout << "name: " << name << std::endl;
  }

  db_connection.close();

  return 0;
}
