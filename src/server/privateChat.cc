#include "./server.h"

void privateChat(FdToName &clients,int currentClientFd, const string &msg, string clientName) {
  auto msgReceiver = find_if(clients.begin(),
  clients.end(),
  [&clientName](auto const &itr) -> bool {
    return itr.second == clientName;
  });
  if (msgReceiver != clients.end()) {
    send(msgReceiver->first, msg.c_str(), msg.size(), 0);
  } else {
    broadcast(clients, currentClientFd, msg);
  }
}
