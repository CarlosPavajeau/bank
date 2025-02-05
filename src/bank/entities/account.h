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

  account(const uint64 id,
          std::string username,
          std::string password,
          std::string email,
          const double balance,
          const uint64 created_at)
      : id(id)
      , username(std::move(username))
      , password(std::move(password))
      , email(std::move(email))
      , balance(balance)
      , created_at(created_at)
  {
  }

  account(std::string username, std::string password, std::string email)
      : username(std::move(username))
      , password(std::move(password))
      , email(std::move(email))
  {
  }

  uint64 id = 0;
  std::string username;
  std::string password;
  std::string email;
  double balance = 0.0;

  uint64 created_at = 0;
};

struct account_transaction
{
  uint64 id;
  double amount;
  std::string kind;

  std::string account_id;
};
}  // namespace bank::entities

#endif  // ACCOUNT_H
