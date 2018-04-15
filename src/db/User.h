#ifndef SRC_DB_USER_H_
#define SRC_DB_USER_H_

#include <string>
#include "./entry/entry.h"

// putting this here compiler shouts "can't put static of Keys in class"
namespace _User {
  const Entry::Keys _keys = {"name", "password", "status"};
  const size_t _sz = 128;
}

class User : public Entry {
 public:
  // default constructor. use "set()" later
  User() : Entry(_User::_sz, _User::_keys) {}

  // order of values must be same as keys.
  explicit User(const Entry::Values &values);

  // de-serialize
  explicit User(const std::string &str) : Entry(_User::_sz, str) {}
};

#endif  // SRC_DB_USER_H_
