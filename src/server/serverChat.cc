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
        cout << color::red << "[CONNECTION:ERROR] " << color::reset
          << "Cannot Accept from newClient = " << newClient << endl;
        continue;
      }
      cout << color::yellow << "[CONNECTION:INFO] " << color::reset
        << "New client joined from <" << getPeerName(newClient) << ">" << endl;
      FD_SET(newClient, &master);
      clients[newClient] = "";
      continue;
    }

    for (auto &p : clients) {
      const int currentClientFd = p.first;
      if (!FD_ISSET(currentClientFd, &master)) continue;

      std::fill_n(buffer, BUFFER_SIZE, '\0');
      const int bytesRead = recv(currentClientFd, buffer, sizeof(buffer), 0);
      cout << "BUFFER = " << buffer << endl;

      bool connectionLost = (bytesRead <= 0);
      if (connectionLost) {
        FD_CLR(currentClientFd, &master);
        close(currentClientFd);
        string username(clients[currentClientFd]);
        logout(db, username);
        clients.erase(currentClientFd);
        string info = "INFO" + DELIM + username + DELIM + "went offline";
        cout << color::magenta << "[CLIENT:OFFLINE] " << color::reset
          << "<" << username << "> went offline" << endl;
        
        for (auto &room : chatRooms) {
          auto members = room.second;
          auto receiverItr = members.find(currentClientFd);
          if (receiverItr != members.end()) {
            room.second.erase(currentClientFd);
            broadcast(members, currentClientFd, info);
          }
        }
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
        auto tokens = split(msg, DELIM, 2);
        auto chatRoomName = tokens[0];
        auto text = tokens[1];
        // TODO: send private messages
        if (chatRooms.count(chatRoomName) != 0) {
          if (chatRooms.at(chatRoomName).count(currentClientFd) != 0) {
            auto msgToSend = "MSG" + DELIM + clients[currentClientFd] + "#" + chatRoomName + DELIM + text;
            auto index = text.find('@');
            
            if (index != string::npos) {
              std::string receiverName = text.substr(index + 1, text.substr(index + 1).find(' '));
              auto receiverItr = find_if(
                clients.begin(),
                clients.end(),
                [&receiverName](auto const &itr) -> bool {
                  return itr.second == receiverName;
                }
              );
              if (receiverItr != clients.end() && chatRooms.at(chatRoomName).count(receiverItr->first) != 0) {
                send(receiverItr->first, msgToSend.c_str(), msgToSend.size(), 0);
                continue;
              }
            }
            broadcast(chatRooms.at(chatRoomName), currentClientFd, msgToSend);
          } else {
            std::string errorMsg = "ERROR" + DELIM + "SERVER" + DELIM + "#" + chatRoomName + " must join chat-room first.";
            send(currentClientFd, errorMsg.c_str(), errorMsg.size(), 0);
          }
        } else {
          std::string errorMsg = "ERROR" + DELIM + "SERVER" + DELIM + "#" + chatRoomName + " doesn't exist.";
          send(currentClientFd, errorMsg.c_str(), errorMsg.size(), 0);
        }
        continue;
      }

      switch (type) {
        case JOIN: {
          auto &chatRoomName = split(msg, DELIM, 2)[1];
          cout << color::green << "[CLIENT:JOIN] " << color::reset
            << "<" << clients[currentClientFd] << "> requested to join #"
            << chatRoomName << endl;
          joinChatRoom(chatRoomName, currentClientFd, clients, chatRooms);
          continue;
        }

        case LEAVE: {
          auto &chatRoomName = split(msg, DELIM, 2)[1];
          cout << color::green << "[CLIENT:LEAVE] " << color::reset
            << "<" << clients[currentClientFd] << "> left #"
            << chatRoomName << endl;
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
