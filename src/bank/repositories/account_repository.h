#ifndef ACCOUNT_REPOSITORY_H
#define ACCOUNT_REPOSITORY_H
#include <optional>

#include "account.h"

namespace bank
{

namespace db
{
class db_connection;
}

class account_repository
{
public:
  explicit account_repository(db::db_connection* db);
  ~account_repository() = default;

  [[nodiscard]] bool save_account(const entities::account& account) const;
  [[nodiscard]] std::optional<entities::account> find_account(uint64 id) const;

private:
  db::db_connection* db_;
};
}  // namespace bank

#endif  // ACCOUNT_REPOSITORY_H
