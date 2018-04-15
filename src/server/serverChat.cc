#include "./server.h"

int serverChat(int sockfd) {
  int max = sockfd;
  char buffer[BUFFER_SIZE];
  FdToName clients;
  ChatRoomToFd chatRooms;
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
          string msg = clients[currentClientFd] + " is offline";
          clients.erase(currentClientFd);
          broadcast(clients, currentClientFd, msg);
        } else if (clients[currentClientFd].empty()) {
          // broadcast new connection info
          string newClientName(buffer);
          string msg = string(newClientName) + " is online";
          clients[currentClientFd] = newClientName;
          broadcast(clients, currentClientFd, msg);
          //sending information to newly connected client.
          firstMsg(chatRooms, currentClientFd);
        } else {
          string msg(buffer);
          unsigned int index = msg.find(' ');
          cout << msg << endl;
          if(++index == msg.size())
            continue;
          string msgToSend(msg.substr(index));
          cout << msgToSend << endl;
          auto  posAt = find(msgToSend.begin(), msgToSend.end(), '@');
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
          else if(msgToSend.find(".create#") == 0  && msgToSend.find(' ') == std::string::npos) {
            string chatRoomName(msgToSend.substr(8));
            createChatRoom(chatRoomName, currentClientFd, clients, chatRooms);
            continue;
          }
          else if(msgToSend.find('#') == 0) {
              posAt = find(msgToSend.begin(), msgToSend.end(), '#');
              string chatRoomName(++posAt, std::find(msgToSend.begin(), msgToSend.end(), ' '));
              broadcastToChatRoom(clients, chatRooms, chatRoomName, currentClientFd, msg);
              continue;
          }
          broadcast(clients, currentClientFd, temp);
        }
      }
    }
  }
  return 0;
}
