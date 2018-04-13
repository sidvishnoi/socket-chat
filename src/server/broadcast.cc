#include "./server.h"

// send msg to all clients accept currentClientFD
void broadcast(FdToName &clients, int currentClientFd, const string &msg) {
  for (auto itr = clients.begin(); itr != clients.end(); ++itr) {
    if (itr->first != currentClientFd && !clients[itr->first].empty()) {
      send(itr->first, msg.c_str(), msg.size(), 0);
    }
  }
}
