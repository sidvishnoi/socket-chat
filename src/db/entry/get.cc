#include "./entry.h"

std::string Entry::get(const std::string &key) const {
  if (m.count(key) != 1) return "undefined";
  return m.at(key);
}

Entry::Map Entry::get() const {
  return Map(m);
}
