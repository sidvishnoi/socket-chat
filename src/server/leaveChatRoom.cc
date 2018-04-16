#include "./server.h"

void leaveChatRoom(const std::string chatRoomName, const int clientFd, const FdToName &names, ChatroomToFdList &chatRooms) {
  auto clientName = names.at(clientFd);
  auto sendTo = chatRooms[chatRoomName];
  chatRooms[chatRoomName].erase(clientFd);

  std::string response = "INFO" + DELIM + "#" + chatRoomName + DELIM + "you left.";
  send(clientFd, response.c_str(), response.size(), 0);

  std::string msg = "INFO" + DELIM + clientName + "#" + chatRoomName + DELIM + "left chatroom.";
  broadcast(sendTo, clientFd, msg);
}
