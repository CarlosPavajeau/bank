#include <gtest/gtest.h>

#include "database/db_connection.h"

TEST(bank, init_db)
{
  bank::db::sqlite3_connection connection(":memory:");

  ASSERT_EQ(connection.open(), true) << "Failed to open db connection";
}

TEST(bank, close_db)
{
  bank::db::sqlite3_connection connection(":memory:");

  ASSERT_EQ(connection.open(), true) << "Failed to open db connection";
  ASSERT_EQ(connection.close(), true) << "Failed to close db connection";
}
