#include "server.h"

void Bind(int sockfd, int port) {
  struct sockaddr_in server_addr;
  bzero((char*)&server_addr, sizeof server_addr);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  int res = bind(sockfd, (struct sockaddr*)&server_addr, sizeof server_addr);
  if (res < 0) {
    printError();
    throw std::runtime_error("Error in binding socket");
  }
}
