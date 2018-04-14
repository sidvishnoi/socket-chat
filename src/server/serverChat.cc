#include "./server.h"

int serverChat(int sockfd) {
  Database<User> db("./data/users/", "name");
  int max = sockfd;
  char buffer[BUFFER_SIZE];
  FdToName clients;
  fd_set master;

  while (true) {
    FD_ZERO(&master);
    FD_SET(sockfd, &master);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
      FD_SET(it->first, &master);
      if (it->first > max) max = it->first;
    }
    select(max+1, &master, NULL, NULL, NULL);

    if (FD_ISSET(sockfd, &master)) {
      const int newClient = Accept(sockfd);
      if (newClient < 0) {
        cout << "Cannot Accept from newClient = " << newClient << endl;
        continue;
      }
      cout << "New client joined: " << getPeerName(newClient) << endl;
      FD_SET(newClient, &master);
      clients[newClient] = "";
      continue;
    }

    for (auto it = clients.begin(); it != clients.end(); ++it) {
      const int currentClientFd = it->first;
      if (FD_ISSET(currentClientFd, &master)) {
        memset(buffer, 0, BUFFER_SIZE);
        const int bytesRead = recv(currentClientFd, buffer, sizeof(buffer), 0);
        cout << "BUFFER = " << buffer << endl;
        if (bytesRead <= 0) {
          // handle client connection lost
          FD_CLR(currentClientFd, &master);
          close(currentClientFd);
          string username(clients[currentClientFd]);
          logout(db, username);
          clients[currentClientFd] = "";
          string info = "INFO" + DELIM + username + DELIM + "went offline";
          broadcast(clients, currentClientFd, info);
        } else if (clients[currentClientFd].empty()) {
          // login new client
          vector<string> tokens = split(string(buffer), DELIM);
          string newClientName(tokens[0]);
          string password(tokens[1]);
          auto loginStatus = login(db, newClientName, password);
          string loginMsg = ((loginStatus != "SUCCESS") ? "ERROR" : "INFO") + DELIM + "AUTH" + DELIM + loginStatus;
          send(currentClientFd, loginMsg.c_str(), loginMsg.size(), 0);
          if (loginStatus != "SUCCESS") {
            cout << "Invalid login attempt by: " << newClientName << endl;
            clients[currentClientFd] = "";
            FD_CLR(currentClientFd, &master);
            close(currentClientFd);
            continue;
          }
          clients[currentClientFd] = newClientName;
          string newClientOnlineMsg = "INFO" + DELIM + newClientName + DELIM +  + "is online";
          broadcast(clients, currentClientFd, newClientOnlineMsg);
        } else {
          // send messages
          bool userFound = false;
          string msg(buffer);
          string msgBody = "MSG" + clients[currentClientFd] + DELIM + msg;
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
            send(msgReceiver->first, msgBody.c_str(), msgBody.size(), 0);
          }
          if (userFound) continue;
          // send to all
          broadcast(clients, currentClientFd, msgBody);
        }
      }
    }
  }
  return 0;
}
