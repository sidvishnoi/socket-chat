#include "./entry.h"

bool Entry::set(const std::string &key, const std::string &value) {
  if (m.count(key) != 1) return false;
  m[key] = value;
  return true;
}

void Entry::insert(const std::string &key, const std::string &value) {
  m[key] = value;
}
