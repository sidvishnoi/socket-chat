#include "./entry.h"

std::string Entry::serialize() const {
  std::string s;
  s.reserve(sz);
  for (const auto &p : m) {
    s += p.first + ":" + p.second + ",";
  }
  if (s.size() != 0) s.erase(--s.end());
  if (s.size() > sz) {
    throw std::length_error("Cannot serialize");
  }
  return s;
}
