#include <format>
#include <iomanip>
#include <iostream>

#include <sqlite3.h>

#include "account.h"
#include "account_repository.h"
#include "db_connection.h"
#include "db_updater.h"
#include "git_revision.h"
#include "logger.h"
#include "password_hasher.h"

int main()
{
  LOG_INFO("git hash are {}", git_revision::get_hash());

  const std::string password = "admin";
  std::string hashed_password;

  bank::crypto::password_hasher::encrypt(password, hashed_password);

  bank::db::db_connection db_connection("test.db");

  if (!db_connection.open()) {
    LOG_ERROR("failed to open database connection");
    return EXIT_FAILURE;
  }

  bank::db::db_updater::update(db_connection);
  db_connection.prepare_statements();

  const bank::account_repository account_repository(&db_connection);
  const bank::entities::account account {
      0, "admin", hashed_password, "admin@admin.com", 0};

  if (const auto save_result = account_repository.save_account(account);
      !save_result)
  {
    LOG_ERROR("failed to save account");
  }

  if (const auto found_account = account_repository.find_account(2);
      !found_account)
  {
    LOG_ERROR("account not found");
  } else {
    LOG_INFO(
        "account {}, email {}", found_account->username, found_account->email);
  }

  // insert new account for employee
  const std::string employee_password = "employee";
  hashed_password.clear();
  bank::crypto::password_hasher::encrypt(employee_password, hashed_password);

  const bank::entities::account employee = {
      0, "employee", hashed_password, "employee@employee.com", 0};

  if (const auto save_employee_result =
          account_repository.save_account(employee);
      !save_employee_result)
  {
    LOG_ERROR("failed to save employee account");
  }

  db_connection.close();

  return 0;
}
