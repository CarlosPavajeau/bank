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
    return std::nullopt;
  }

  if (!result->next()) {
    return std::nullopt;
  }

  const auto row = result->fetch();

  auto account = from_row(row);

  delete result;  // free memory

  return std::make_optional(account);
}

entities::account account_repository::from_row(const db::field* row)
{
  const auto id = static_cast<uint64>(row[0].get_int());
  const auto username = row[1].get_string();
  const auto password = row[2].get_string();
  const auto email = row[3].get_string();
  const auto balance = row[4].get_decimal();
  const auto created_at = static_cast<uint64>(row[5].get_int());

  entities::account account(id, username, password, email, balance, created_at);

  return account;
}

}  // namespace bank
