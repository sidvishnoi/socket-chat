#ifndef SRC_SERVER_USER_H_
#define SRC_SERVER_USER_H_

#include <string>
#include "./entry/entry.h"

class User : public Entry {
  static const size_t _sz = 128;
 public:
  User() : Entry(_sz, Keys{"name", "password", "host"}) {}
  explicit User(const Entry::Keys &values) : Entry(_sz) {
    insert("name", values[0]);
    insert("password", values[1]);
    insert("host", values[2]);
  }
  explicit User(const std::string &str) : Entry(_sz, str) {}
};

#endif  // SRC_SERVER_USER_H_
