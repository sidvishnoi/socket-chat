#include "./entry.h"

std::string Entry::serialize() const {
  std::string s;
  s.reserve(sz);
  for (const auto &p : m) {
    s += p.first + ":" + p.second + ",";
  }
  if (s.size() != 0) s.erase(--s.end());  // erase trailing comma
  if (s.size() > sz) {
    throw std::length_error("Cannot serialize");
  }
  std::string pad(sz - s.size(), ' ');
  s.append(pad);
  return s;
}
