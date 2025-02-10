#include <format>

#include <sqlite3.h>

#include "account.h"
#include "account_manager.h"
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

  bank::db::db_updater::populate(db_connection);

  bank::db::db_updater::update(db_connection);
  db_connection.prepare_statements();

  const bank::account_manager account_manager(&db_connection);

  bank::entities::account admin_account(
      "admin", hashed_password, "admin@admin.com");

  if (const auto save_result = account_manager.save(admin_account);
      !save_result)
  {
    LOG_ERROR("failed to save account admin");
  }

  const std::string employee_password = "employee";
  hashed_password.clear();
  bank::crypto::password_hasher::encrypt(employee_password, hashed_password);

  bank::entities::account employee_account(
      "employee", hashed_password, "employee@employee.com");

  if (const auto save_employee_result = account_manager.save(employee_account);
      !save_employee_result)
  {
    LOG_ERROR("failed to save account employee");
  }

  if (const auto found_account = account_manager.find(1); !found_account) {
    LOG_ERROR("account not found");
  } else {
    LOG_INFO("account {}, email {}, balance {}",
             found_account->username,
             found_account->email,
             found_account->balance);
  }

  if (const auto found_account = account_manager.find(2); !found_account) {
    LOG_ERROR("account not found");
  } else {
    LOG_INFO("account {}, email {}, balance {}",
             found_account->username,
             found_account->email,
             found_account->balance);
  }

  const bank::entities::account_transaction transaction(
      100, bank::entities::account_transaction_kind::in, 1);
  if (const auto make_transaction_result =
          account_manager.make_transaction(transaction);
      !make_transaction_result)
  {
    LOG_ERROR("failed to make transaction");
  }

  db_connection.close();

  return 0;
}
