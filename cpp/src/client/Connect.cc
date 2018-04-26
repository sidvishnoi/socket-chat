#include "./client.h"

int Connect(int sockfd, const std::string &host, int port) {
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  inet_pton(AF_INET, host.c_str(), &(server_addr.sin_addr));

  const int res = connect(
    sockfd,
    reinterpret_cast<struct sockaddr*>(&server_addr),
    sizeof(server_addr));
  if (res == -1) {
    throw std::runtime_error("Error in making connection");
  }
  return 0;
}
