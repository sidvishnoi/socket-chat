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
        const int bytesRead = recv(currentClientFd, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
          // handle client connection lost
          FD_CLR(currentClientFd, &master);
          close(currentClientFd);
          clients[currentClientFd] = "";
          string msg = clients[currentClientFd] + " is offline";
          broadcast(clients, currentClientFd, msg);
        } else if (clients[currentClientFd].empty()) {
          // broadcast new connection info
          string newClientName(buffer);
          string msg = string(newClientName) + " is online";
          clients[currentClientFd] = newClientName;
          broadcast(clients, currentClientFd, msg);
        } else {
          // send messages
          bool userFound = false;
          string msg(buffer);
          cout << msg << endl;
          auto posAt = find(msg.begin(), msg.end(), '@');
          if (posAt == msg.begin()) {
            // send private message
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
          // send to all
          string temp = clients[currentClientFd] + " > " + msg;
          broadcast(clients, currentClientFd, temp);
        }
      }
    }
  }
  return 0;
}
