#include "password_hasher.h"

#include <sodium.h>

namespace bank::crypto::password_hasher
{
bool encrypt(const std::string_view password, std::string& hashed_password)
{
  char buffer[crypto_pwhash_STRBYTES];

  const auto hash_result = crypto_pwhash_str(buffer,
                                             password.data(),
                                             password.size(),
                                             crypto_pwhash_OPSLIMIT_SENSITIVE,
                                             crypto_pwhash_MEMLIMIT_MIN);
  if (hash_result != 0) {
    return false;
  }

  hashed_password = std::string(buffer);

  return true;
}

bool verify(const std::string_view hashed_password,
            const std::string_view password)
{
  const auto verify_result = crypto_pwhash_str_verify(
      hashed_password.data(), password.data(), password.size());

  return verify_result == 0;
}

}  // namespace bank::crypto::password_hasher
