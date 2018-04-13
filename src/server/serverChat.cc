#include "./server.h"


int serverChat(int sockfd) {
  int max = sockfd;
  char buffer[BUFFER_SIZE];
  FdToName clients;
  fd_set master;

  while (true) {
    FD_ZERO(&master);
    FD_SET(sockfd, &master);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
      FD_SET(it->first, &master);
      if (it->first > max)
        max = it->first;
    }
    select(max+1, &master, NULL, NULL, NULL);

    if (FD_ISSET(sockfd, &master)) {
      const int newClient = Accept(sockfd);
      cout << newClient << " Accepted new connection" << endl;
      if (newClient < 0) {
        cout << "Cannot Accept from newClient" << endl;
        continue;
      }
      FD_SET(newClient, &master);
      clients[newClient] = "";
      send(newClient, welcomeMsg.c_str(), welcomeMsg.size(), 0);
      continue;
    }
    for (auto it = clients.begin(); it != clients.end(); ++it) {
      const int currentClientFd = it->first;
      if (FD_ISSET(currentClientFd, &master)) {
        memset(buffer, 0, BUFFER_SIZE);
        // handle client connection lost
        const int bytesRead = recv(currentClientFd, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
          FD_CLR(currentClientFd, &master);
          close(currentClientFd);
          string msg = clients[currentClientFd] + " is offline";
          if (clients[currentClientFd] != "") {
            for (auto itr = clients.begin(); itr != clients.end(); ++itr) {
              if (itr->first != currentClientFd) {
                // broadcast to all except the one lost
                send(itr->first, msg.c_str(), msg.size(), 0);
              }
            }
          }
        } else if (clients[currentClientFd].empty()) {
          // broadcast new connection info
          string newClientName(buffer);
          string msg = string(newClientName) + " is online";
          for (auto itr = clients.begin(); itr != clients.end(); ++itr) {
            if (itr->first != currentClientFd) {
              // broadcast to all except the new one
              send(itr->first, msg.c_str(), msg.size(), 0);
            } else {
              clients[currentClientFd] = newClientName;
            }
          }
        } else {
          bool userFound = false;
          string msg(buffer);
          cout << msg << endl;
          auto posAt = find(msg.begin(), msg.end(), '@');
          if (posAt == msg.begin()) {
            string clientName(++posAt, std::find(msg.begin(), msg.end(), ' '));
            auto msgReceiver = find_if(clients.begin(),
              clients.end(),
              [&clientName](auto const &itr) -> bool {
                return itr.second == clientName;
            });
            if (msgReceiver != clients.end()) userFound = true;
            string temp = clients[currentClientFd] + " > " + msg;
            send(msgReceiver->first, temp.c_str(), temp.size(), 0);
          }
          if (userFound) continue;
          string temp = clients[currentClientFd] + " > " + msg;
          for (auto itr = clients.begin(); itr != clients.end(); ++itr) {
            if (itr->first != currentClientFd && !clients[itr->first].empty()) {
              send(itr->first, temp.c_str(), temp.size(), 0);
            }
          }
        }
      }
    }
  }
  return 0;
}
