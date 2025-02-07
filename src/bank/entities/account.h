#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <utility>

#include "define.h"

namespace bank::entities
{
struct account
{
  account() = default;

  account(const uint64 id_,
          std::string username_,
          std::string password_,
          std::string email_,
          const double balance_,
          const uint64 created_at_)
      : id(id_)
      , username(std::move(username_))
      , password(std::move(password_))
      , email(std::move(email_))
      , balance(balance_)
      , created_at(created_at_)
  {
  }

  account(std::string username_, std::string password_, std::string email_)
      : username(std::move(username_))
      , password(std::move(password_))
      , email(std::move(email_))
  {
  }

  uint64 id = 0;
  std::string username;
  std::string password;
  std::string email;
  double balance = 0.0;

  uint64 created_at = 0;
};

enum class account_transaction_kind : uint8
{
  in,
  out
};

inline std::string_view to_string(const account_transaction_kind kind) noexcept
{
  switch (kind) {
    case account_transaction_kind::in:
      return "IN";
    case account_transaction_kind::out:
      return "OUT";
  }
  return "UNKNOWN";
}

struct account_transaction
{
  account_transaction() = default;

  account_transaction(const uint64 id_,
                      const double amount_,
                      const account_transaction_kind kind_,
                      const uint64 account_id_)
      : id(id_)
      , amount(amount_)
      , kind(kind_)
      , account_id(account_id_)
  {
  }

  account_transaction(const double amount_,
                      const account_transaction_kind kind_,
                      const uint64 account_id_)
      : amount(amount_)
      , kind(kind_)
      , account_id(account_id_)
  {
  }

  uint64 id = 0;
  double amount = 0;
  account_transaction_kind kind = account_transaction_kind::in;

  uint64 account_id = 0;
};
}  // namespace bank::entities

#endif  // ACCOUNT_H
