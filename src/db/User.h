#ifndef SRC_DB_USER_H_
#define SRC_DB_USER_H_

#include <string>
#include "./entry/entry.h"

const Entry::Keys _keys = {"name", "password", "host"};

class User : public Entry {
 private:
  static const size_t _sz = 128;

 public:
  // default constructor. use "set()" later
  User() : Entry(_sz, _keys) {}

  // order of values must be same as keys.
  explicit User(const Entry::Values &values);

  // de-serialize
  explicit User(const std::string &str) : Entry(_sz, str) {}
};

User::User(const Entry::Values &values) : Entry(_sz) {
  if (values.size() != _keys.size()) {
    throw std::invalid_argument(
      "Failed to create instance of `User(Values values)`"
      ": size of keys must be same as size of values");
  }
  for (size_t k = 0; k < _keys.size(); ++k) {
    insert(_keys[k], values[k]);
  }
}

#endif  // SRC_DB_USER_H_
