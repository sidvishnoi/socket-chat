#include "server.h"

int Accept(int sockfd) {
  struct sockaddr_in client_addr;
  socklen_t len = static_cast<socklen_t>(sizeof client_addr);
  const int clientSockfd = accept(sockfd, (struct sockaddr*)&client_addr, &len);
  if (clientSockfd < 0) {
    printError();
    throw std::runtime_error("Error on accept");
  }
  return clientSockfd;
}
