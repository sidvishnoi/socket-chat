#include "./client.h"

bool login(const int sockfd, const StrPair &credentials) {
  // send credentials to server for login
  std::string cred = credentials.first + DELIM + credentials.second;
  send(sockfd, cred.c_str(), cred.size(), 0);

  char buffer[BUFFER_SIZE] = {0};
  if (recv(sockfd, buffer, BUFFER_SIZE, 0) <= 0) {
    throw std::runtime_error("Authentication failed.");
  }

  std::string response(buffer);
  if (response.find("INFO") == 0) return true;

  printMessage(response);
  throw std::runtime_error("Authentication failed.");
}
