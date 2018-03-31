#include "./server.h"

void Bind(int sockfd, int port) {
  struct sockaddr_in server_addr;
  bzero(reinterpret_cast<char*>(&server_addr), sizeof server_addr);

  const int reuse = 1;
  if (setsockopt(
      sockfd,
      SOL_SOCKET,
      SO_REUSEADDR,
      reinterpret_cast<const char*>(&reuse),
      sizeof(reuse)) < 0) {
    throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  int res = bind(sockfd,
      reinterpret_cast<struct sockaddr*>(&server_addr),
      sizeof server_addr);
  if (res < 0) {
    printError();
    throw std::runtime_error("Error in binding socket");
  }
}
