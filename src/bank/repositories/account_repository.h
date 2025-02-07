#ifndef ACCOUNT_REPOSITORY_H
#define ACCOUNT_REPOSITORY_H
#include <optional>

#include "account.h"
#include "database_env.h"

namespace bank
{

class account_repository
{
public:
  explicit account_repository(db::db_connection* db);
  ~account_repository() = default;

  [[nodiscard]] bool save_account(const entities::account& account) const;
  [[nodiscard]] std::optional<entities::account> find_account(uint64 id) const;

  [[nodiscard]] bool delete_account(uint64 id) const;
  [[nodiscard]] bool make_transaction(
      const entities::account_transaction& transaction) const;

  // [[nodiscard]] bool check_password(uint64 id, std::string_view password)
  // const;

private:
  db::db_connection* db_;

  static entities::account from_row(const db::field* row);
};
}  // namespace bank

#endif  // ACCOUNT_REPOSITORY_H
