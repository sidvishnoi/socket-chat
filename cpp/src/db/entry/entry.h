#ifndef SRC_DB_ENTRY_ENTRY_H_
#define SRC_DB_ENTRY_ENTRY_H_

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>

class Entry {
 public:
  using Map = std::map<std::string, std::string>;
  using Keys = std::vector<std::string>;
  using Values = std::vector<std::string>;

  explicit Entry(size_t _sz) : sz(_sz) {}      // used with "insert()"
  Entry(size_t _sz, const Keys &keys);         // empty object. use with "set()"
  Entry(size_t _sz, const std::string& str);   // deserialize
  Entry(const Entry &other);                   // copy constructor

  // add a new key-value pair
  void insert(const std::string &key, const std::string &value);

  // set an existing key to a new value
  bool set(const std::string &key, const std::string &value);

  // get value correspondoing to a key
  std::string get(const std::string &key) const;

  // get entire public key-value pairs
  Map get() const;

  // convert the object to `key:value,` string
  std::string serialize() const;

  // return the max size of serialisable string
  size_t size() const { return sz; }

 private:
  size_t sz;
  Map m;
};

#endif  // SRC_DB_ENTRY_ENTRY_H_
