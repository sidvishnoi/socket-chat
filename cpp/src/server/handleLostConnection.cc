#include "./server.h"

void handleLostConnection(const int currentClientFd,
  ChatroomToFdList &chatRooms, 
  FdToName &clients, 
  fd_set &master, 
  Database<User> &db
  ) {
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
    //Check if current list is the member of the chatroom.
    if (members.find(currentClientFd) != members.end()) {
      room.second.erase(currentClientFd);
      broadcast(members, currentClientFd, info);
    }
  }
}