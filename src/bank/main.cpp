#include <iomanip>
#include <iostream>

#include "db_connection.h"
#include "field.h"
#include "query_result.h"
#include "sqlite3.h"

int main()
{
  bank::db::db_connection db_connection("test.db");

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

  const auto query_result = db_connection.query("SELECT * FROM test");

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
