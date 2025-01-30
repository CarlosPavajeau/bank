#include "account_repository.h"

#include "db_connection.h"
#include "field.h"
#include "prepared_statement.h"
#include "query_result.h"
#include "sqlite3.h"

namespace bank
{
account_repository::account_repository(db::db_connection* db)
    : db_(db)
{
}

bool account_repository::save_account(const entities::account& account) const
{
  const auto insert_stmt = db_->get_prepared_statement(db::insert_account);

  insert_stmt->set_string(0, account.username);
  insert_stmt->set_string(1, account.password);
  insert_stmt->set_string(2, account.email);

  const auto result = db_->execute(insert_stmt);

  return result == SQLITE_DONE;
}

std::optional<entities::account> account_repository::find_account(
    const uint64 id) const
{
  const auto select_stmt = db_->get_prepared_statement(db::select_account);

  select_stmt->set_int(0, static_cast<int>(id));

  const auto result = db_->query(select_stmt);

  if (result == nullptr) {
    return {};
  }

  if (!result->next()) {
    return {};
  }

  const auto row = result->fetch();

  entities::account account {
      static_cast<uint64>(row[0].get_int()),
      row[1].get_string(),
      row[2].get_string(),
      row[3].get_string(),
      static_cast<uint64>(row[4].get_int()),
  };

  return account;
}

}  // namespace bank
