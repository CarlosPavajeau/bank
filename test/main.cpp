#include <gtest/gtest.h>

#include "database/db_connection.h"
#include "query_result.h"
#include "sqlite3.h"

TEST(database, init_db)
{
  bank::db::db_connection connection(":memory:");

  ASSERT_EQ(connection.open(), true) << "Failed to open db connection";
}

TEST(database, close_db)
{
  bank::db::db_connection connection(":memory:");

  ASSERT_EQ(connection.open(), true) << "Failed to open db connection";
  ASSERT_EQ(connection.close(), true) << "Failed to close db connection";
}

TEST(database, perform_a_query)
{
  bank::db::db_connection connection(":memory:");

  ASSERT_EQ(connection.open(), true) << "Failed to open db connection";

  const int create_table_result = connection.execute(
      "CREATE TABLE IF NOT EXISTS test(id int, name VARCHAR(32))");
  ASSERT_EQ(create_table_result, SQLITE_OK) << "Failed to create test table";

  const int insert_result = connection.execute(
      R"(INSERT INTO test VALUES (1, "Manolo"), (2, "Pedro"))");
  ASSERT_EQ(insert_result, SQLITE_OK) << "Failed to insert test values";

  const auto query_result = connection.query("SELECT * FROM test");

  ASSERT_EQ(query_result->next(), true) << "Failed to query data";
  ASSERT_EQ(query_result->next(), true) << "Failed to query data";

  ASSERT_EQ(query_result->next(), false) << "Failed to query data";

  ASSERT_EQ(connection.close(), true) << "Failed to close db connection";
}
