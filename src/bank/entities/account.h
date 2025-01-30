#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>

#include "define.h"

namespace bank::entities
{
struct account
{
  uint64 id;
  std::string username;
  std::string password;
  std::string email;
  uint64 created_at;
};

struct account_balance
{
  uint64 id;
  double balance;

  std::string account_id;
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
