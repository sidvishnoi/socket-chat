#include "./User.h"

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
