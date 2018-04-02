#ifndef SRC_SERVER_DB_ENTRY_ENTRY_H_
#define SRC_SERVER_DB_ENTRY_ENTRY_H_

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>
using std::cout; using std::endl;

class Entry {
 public:
  using Map = std::map<std::string, std::string>;
  using Keys = std::vector<std::string>;
  explicit Entry(size_t maxLen) : sz(maxLen) {}
  Entry(size_t maxLen, const Keys &keys);
  Entry(size_t maxLen, const std::string& str);
  Entry(const Entry &other);

  void insert(const std::string &key, const std::string &value);
  bool set(const std::string &key, const std::string &value);
  std::string get(const std::string &key) const;
  Map get() const;
  std::string serialize() const;
  size_t size() { return sz; }

 private:
  size_t sz;
  Map m;
};

#endif  // SRC_SERVER_DB_ENTRY_ENTRY_H_
