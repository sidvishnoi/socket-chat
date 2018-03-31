#include "./server.h"

int Socket() {
  const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    throw std::runtime_error("Error opening socket");
  }
  return sockfd;
}
