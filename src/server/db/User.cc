#include "./db.h"

User::User(const std::string& str) {
  using std::stringstream;
  using std::string;
  using std::getline;
  using std::map;

  stringstream in(str);
  string token, key, value;
  map<string, string> m;
  while(getline(in, token, ',')) {
    stringstream tk(token);
    getline(getline(tk, key, ':') >> std::ws, value);
    m[key] = value;
  }

  username = m["name"];
  password = m["pass"];
  sockfd = std::stoi(m["fd"]);
  host = m["host"];
  port = std::stoi(m["port"]);
}

std::string User::serialize() {
  std::string s;
  s.reserve(128);
  s += "name:" + username + ",";
  s += "pass:" + password + ",";
  s += "fd:" + std::to_string(sockfd) + ",";
  s += "host:" + host + ",";
  s += "port:" + std::to_string(port);
  return s;
}
