#include "./server.h"

int serverChat(int sockfd) {
  using namespace cmd;
  Database<User> db("./data/users/", "name");
  int max = sockfd;
  char buffer[BUFFER_SIZE];
  FdToName clients;
  ChatroomToFdList chatRooms;
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
        bool connectionLost = (bytesRead <= 0);
        if (connectionLost) {
          FD_CLR(currentClientFd, &master);
          close(currentClientFd);
          string username(clients[currentClientFd]);
          logout(db, username);
          clients[currentClientFd] = "";
          string info = "INFO" + DELIM + username + DELIM + "went offline";
          // TODO:
          // 1. find groups in which client was,
          // 2. broadcast to all those groups
          // broadcast(clients, currentClientFd, info);
        } else if (clients[currentClientFd].empty()) {
          // login new client
          vector<string> tokens = split(string(buffer), DELIM, 2);
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
        } else {
          // process messages
          string msg(buffer);
          auto type = getMessageType(msg);
          if (type == NOT_CMD) {
            // TODO: send messages
            cout << "message: " << msg << endl;
          }
          switch (type) {
            case JOIN: {
              auto &chatRoomName = split(msg, DELIM, 2)[1];
              cout << clients[currentClientFd] << " requested to join " << chatRoomName << endl;
              joinChatRoom(chatRoomName, currentClientFd, clients, chatRooms);
              continue;
            }

            case LIST_CHATROOMS: {
              // send list of chatrooms to newly connected client.
              auto response = getChatroomsList(chatRooms);
              send(currentClientFd, response.c_str(), response.size(), 0);
              continue;
            }

            case LIST_PEOPLE: {
              continue;
            }

            case LEAVE: {
              continue;
            }

            case INVALID:
            default: continue;
          }

          continue;
          // DO NOT READ BELOW. IT'S TEMPORARY AND WILL BE REFACTORED.

          unsigned int index = msg.find(' ');
          if(++index == msg.size()) continue;
          string msgToSend(msg.substr(index));
          cout << msgToSend << endl;
          auto posAt = find(msgToSend.begin(), msgToSend.end(), '@');
          string temp = clients[currentClientFd] + " > " + msgToSend;
          if (posAt == msgToSend.begin()) {
            string clientName(++posAt, std::find(msgToSend.begin(), msgToSend.end(), ' '));
            privateChat(clients, currentClientFd, temp, clientName);
            continue;
          }
          else if(msgToSend.find(".join#") == 0 && msgToSend.find(' ') == std::string::npos) {
            string chatRoomName(msgToSend.substr(6));
            joinChatRoom(chatRoomName, currentClientFd, clients, chatRooms);
            continue;
          }
          else if(msgToSend.find('#') == 0) {
              posAt = find(msgToSend.begin(), msgToSend.end(), '#');
              string chatRoomName(++posAt, std::find(msgToSend.begin(), msgToSend.end(), ' '));
              // broadcast(clients, chatRooms, chatRoomName, currentClientFd, msg);
              continue;
          }
          // broadcast(clients, currentClientFd, temp);
        }
      }
    }
  }
  return 0;
}
