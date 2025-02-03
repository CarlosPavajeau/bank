#ifndef PASSWORD_HASHER_H
#define PASSWORD_HASHER_H
#include <string>

namespace bank::crypto::password_hasher
{
bool encrypt(std::string_view password, std::string& hashed_password);
bool verify(std::string_view hashed_password, std::string_view password);
};  // namespace bank::crypto::password_hasher

#endif  // PASSWORD_HASHER_H
