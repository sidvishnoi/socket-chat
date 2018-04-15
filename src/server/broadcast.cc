#include "./server.h"

// send msg to all clients except currentClientFD
void broadcast(
  const FdToName &names,
  const FdList &clients,
  const int currentClientFd,
  const string &msg
  ) {
  auto shouldSend = [&names, currentClientFd](auto client) -> bool {
    return client != currentClientFd
      && names.count(client) != 0
      && !names.at(client).empty();
  };

  auto message = msg.c_str();
  auto len = msg.size();

  for (auto client : clients) {
    if (shouldSend(client)) {
      send(client, message, len, 0);
    }
  }
}
