#ifndef SRC_DB_USER_H_
#define SRC_DB_USER_H_

#include <string>
#include "./entry/entry.h"

// putting this here compiler shouts "can't put static of Keys in class"
namespace _User {
  const Entry::Keys _keys = {"name", "password", "host"};
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

User::User(const Entry::Values &values) : Entry(_User::_sz) {
  if (values.size() != _User::_keys.size()) {
    throw std::invalid_argument(
      "Failed to create instance of `User(Values values)`"
      ": size of keys must be same as size of values");
  }
  for (size_t k = 0; k < _User::_keys.size(); ++k) {
    insert(_User::_keys[k], values[k]);
  }
}

#endif  // SRC_DB_USER_H_
