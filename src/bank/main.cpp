#include <iostream>

#include "db_connection.h"
#include "sqlite3.h"

struct test_result
{
  int id;
};

int test_db_callback(void* data, const int argc, char** argv, char** azColName)
{
  printf("sqlite callback\n");
  for (int i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");

  const auto result = static_cast<test_result*>(data);
  result->id = std::atoi(argv[0]);

  return 0;
}

int main()
{
  sqlite3* db;

  const int open_result = sqlite3_open("test.db", &db);
  if (open_result != SQLITE_OK) {
    std::cout << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return EXIT_FAILURE;
  }

  test_result result {};
  const int exec_result = sqlite3_exec(db,
                                       "SELECT id FROM test where id = 1",
                                       test_db_callback,
                                       &result,
                                       nullptr);

  if (exec_result != SQLITE_OK) {
    std::cout << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return EXIT_FAILURE;
  }

  std::cout << "Resul is: " << result.id << std::endl;

  sqlite3_close(db);

  return 0;
}
