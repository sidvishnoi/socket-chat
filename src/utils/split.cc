#include <vector>
#include <string>

std::vector<std::string> split(const std::string &str, const std::string &delim, const int limit) {
  using std::vector; using std::string;
  vector<string> tokens;
  size_t prev = 0, pos = 0;
  int found = 0;
  do {
    pos = str.find(delim, prev);
    if (pos == string::npos) pos = str.length();
    string token = str.substr(prev, pos-prev);
    if (!token.empty()) {
      tokens.push_back(token);
      ++found;
    }
    prev = pos + delim.length();
  } while ((found < limit) || (pos < str.length() && prev < str.length()));
  return tokens;
}
