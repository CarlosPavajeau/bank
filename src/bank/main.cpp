#include <format>
#include <iomanip>
#include <iostream>

#include <sqlite3.h>

#include "account.h"
#include "account_repository.h"
#include "db_connection.h"
#include "db_updater.h"
#include "git_revision.h"
#include "password_hasher.h"

int main()
{
  std::cout << "[INFO]: git hash: " << git_revision::get_hash() << std::endl;

  const std::string password = "admin";
  std::string hashed_password;

  bank::crypto::password_hasher::encrypt(password, hashed_password);

  bank::db::db_connection db_connection("test.db");

  if (!db_connection.open()) {
    std::cerr << "[ERROR]: failed to open database connection" << std::endl;
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
    std::cerr << "[ERROR]: failed to save account" << std::endl;
  }

  if (const auto found_account = account_repository.find_account(3);
      !found_account)
  {
    std::cerr << "[ERROR]: account not found" << std::endl;
  } else {
    std::cout << std::format("[INFO]: account {}, email {}",
                             found_account->username,
                             found_account->email)
              << std::endl;
  }

  db_connection.close();

  return 0;
}
