#include "./client.h"

std::pair<std::string, std::string> getCredentials() {
  std::string name, password;
  while (true) {
    cout << "Username: ";
    cin >> name;
    if (!name.empty()) break;
  }
  while (true) {
    cout << "Password: ";
    cin >> password;
    if (!password.empty()) break;
  }

  return {name, password};
}
