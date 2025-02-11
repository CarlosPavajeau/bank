#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H
#include <optional>

#include "account.h"
#include "database_env.h"

namespace bank
{

class account_manager
{
public:
  explicit account_manager(db::db_connection* db);
  ~account_manager() = default;

  [[nodiscard]] bool save(const entities::account& account) const;
  [[nodiscard]] std::optional<entities::account> find(uint64 id) const;

  [[nodiscard]] bool remove(uint64 id) const;
  [[nodiscard]] bool make_transaction(
      const entities::account_transaction& account_transaction) const;

  [[nodiscard]] bool check_password(uint64 id, std::string_view password) const;

private:
  db::db_connection* db_;

  static entities::account from_row(const db::field* row);
};
}  // namespace bank

#endif  // ACCOUNT_MANAGER_H
