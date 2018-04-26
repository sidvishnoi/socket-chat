#include "./server.h"

// send msg to all clients except currentClientFD
void broadcast(
  const FdList &clients,
  const int currentClientFd,
  const string &msg
  ) {
  auto message = msg.c_str();
  auto len = msg.size();

  for (auto client : clients) {
    if (client != currentClientFd) {
      send(client, message, len, 0);
    }
  }
}
