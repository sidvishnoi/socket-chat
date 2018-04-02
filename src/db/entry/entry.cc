#include "./entry.h"

Entry::Entry(size_t _sz, const std::string &str) {
  using std::getline;
  using std::map;
  using std::string;
  using std::stringstream;

  sz = _sz;
  stringstream in(str);
  string token, key, value;
  while (getline(in, token, ',') && !token.empty()) {
    stringstream tk(token);
    getline(getline(tk, key, ':') >> std::ws, value);
    m[key] = value;
  }
}

Entry::Entry(const Entry &other) {
  sz = other.sz;
  m.insert(other.m.begin(), other.m.end());
}

Entry::Entry(size_t _sz, const Keys &keys) {
  sz = _sz;
  for (const auto &key : keys) {
    m[key] = "undefined";
  }
}
