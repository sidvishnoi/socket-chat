#include "./server.h"

std::string getPeerName(const int sockfd) {
  struct sockaddr_in address;
  struct sockaddr_in *addrPtr = &address;
  socklen_t len = static_cast<socklen_t>(sizeof address);
  const int err = getpeername(sockfd,
    reinterpret_cast<struct sockaddr*>(&address),
    &len);
  if (err != 0) {
    return "";
  }
  char buf[50] = {0};
  inet_ntop(AF_INET,
    reinterpret_cast<void *>(&addrPtr->sin_addr),
    buf,
    sizeof(buf));
  return std::string(buf) + ":" + std::to_string(ntohs(address.sin_port));
}
