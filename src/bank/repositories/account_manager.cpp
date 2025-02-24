#include "account_manager.h"

#include "db_connection.h"
#include "field.h"
#include "password_hasher.h"
#include "prepared_statement.h"
#include "query_result.h"
#include "sqlite3.h"

namespace bank
{
account_manager::account_manager(db::db_connection* db)
    : db_(db)
{
}

bool account_manager::save(const entities::account& account) const
{
  const auto insert_stmt = db_->get_prepared_statement(db::insert_account);

  insert_stmt->set_string(0, account.username);
  insert_stmt->set_string(1, account.password);
  insert_stmt->set_string(2, account.email);

  const auto result = db_->execute(insert_stmt);

  return result == SQLITE_DONE;
}

std::optional<entities::account> account_manager::find(const uint64 id) const
{
  const auto select_stmt = db_->get_prepared_statement(db::select_account);

  select_stmt->set_uint64(0, id);

  const auto result = db_->query(select_stmt);

  if (result == nullptr) {
    return std::nullopt;
  }

  if (!result->next()) {
    return std::nullopt;
  }

  const auto row = result->fetch();

  auto account = from_row(row);

  return std::make_optional(account);
}

bool account_manager::remove(const uint64 id) const
{
  db_->begin_transaction();

  const auto delete_statement = db_->get_prepared_statement(db::delete_account);
  delete_statement->set_uint64(0, id);

  if (const auto result = db_->execute(delete_statement); result != SQLITE_DONE)
  {
    db_->rollback_transaction();
    return false;
  }

  db_->commit_transaction();

  return true;
}

bool account_manager::make_transaction(
    const entities::account_transaction& account_transaction) const
{
  db_->begin_transaction();

  const auto account = find(account_transaction.account_id);

  if (!account) {
    db_->rollback_transaction();
    return false;
  }

  // check if account has enough balance
  if (account_transaction.kind == entities::account_transaction_kind::out
      && account->balance < account_transaction.amount)
  {
    db_->rollback_transaction();
    return false;
  }

  const auto insert_transaction_statement =
      db_->get_prepared_statement(db::insert_account_transaction);

  insert_transaction_statement->set_uint64(0, account_transaction.account_id);
  insert_transaction_statement->set_double(1, account_transaction.amount);
  insert_transaction_statement->set_string(2,
                                           to_string(account_transaction.kind));

  if (const auto result = db_->execute(insert_transaction_statement);
      result != SQLITE_DONE)
  {
    db_->rollback_transaction();
    return false;
  }

  const auto new_balance =
      account_transaction.kind == entities::account_transaction_kind::in
      ? account->balance + account_transaction.amount
      : account->balance - account_transaction.amount;
  const auto update_balance_statement =
      db_->get_prepared_statement(db::update_account_balance);

  update_balance_statement->set_double(0, new_balance);
  update_balance_statement->set_uint64(1, account_transaction.account_id);

  if (const auto result = db_->execute(update_balance_statement);
      result != SQLITE_DONE)
  {
    db_->rollback_transaction();
    return false;
  }

  db_->commit_transaction();

  return true;
}

bool account_manager::check_password(const uint64 id,
                                     const std::string_view password) const
{
  const auto account = find(id);

  if (!account) {
    return false;
  }

  const auto result =
      crypto::password_hasher::verify(account->password, password);

  return result;
}

entities::account account_manager::from_row(const db::field* row)
{
  const auto id = row[0].get_int64();
  const auto username = row[1].get_string();
  const auto password = row[2].get_string();
  const auto email = row[3].get_string();
  const auto balance = row[4].get_decimal();
  const auto created_at = row[5].get_int64();

  entities::account account(id, username, password, email, balance, created_at);

  return account;
}

}  // namespace bank
