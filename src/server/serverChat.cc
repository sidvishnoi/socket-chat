#include "./server.h"

int serverChat(int sockfd) {
  using namespace cmd;
  Database<User> db("./data/users/", "name");
  int maxfd = sockfd;
  char buffer[BUFFER_SIZE];
  FdToName clients;
  ChatroomToFdList chatRooms;
  fd_set master;

  while (true) {
    FD_ZERO(&master);
    FD_SET(sockfd, &master);
    for (auto &p : clients) {
      FD_SET(p.first, &master);
      if (p.first > maxfd) maxfd = p.first;
    }
    select(maxfd + 1, &master, NULL, NULL, NULL);

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

    for (auto &p : clients) {
      const int currentClientFd = p.first;
      if (!FD_ISSET(currentClientFd, &master)) continue;

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
        continue;
      }

      if (clients[currentClientFd].empty()) {
        addClient(currentClientFd, &master, string(buffer), db, clients);
        continue;
      }

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

        case LEAVE: {
          auto &chatRoomName = split(msg, DELIM, 2)[1];
          cout << clients[currentClientFd] << " left #" << chatRoomName << endl;
          leaveChatRoom(chatRoomName, currentClientFd, clients, chatRooms);
          continue;
        }

        case LIST_CHATROOMS: {
          auto response = getChatroomsList(chatRooms);
          send(currentClientFd, response.c_str(), response.size(), 0);
          continue;
        }

        case LIST_PEOPLE: {
          auto &chatRoomName = split(msg, DELIM, 2)[1];
          auto lst = getPeopleList(chatRoomName, clients, chatRooms);
          auto response = "INFO" + DELIM + "PEOPLE#" + chatRoomName + DELIM + "\n" + lst;
          send(currentClientFd, response.c_str(), response.size(), 0);
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
    } // end:range-for-clients
  }  // end:while
  return 0;
}
