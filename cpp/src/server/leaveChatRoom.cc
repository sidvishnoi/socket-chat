#include "./server.h"

void leaveChatRoom(
  const std::string chatRoomName,
  const int clientFd,
  const FdToName &names,
  ChatroomToFdList &chatRooms
) {
  if (chatRooms.count(chatRoomName) == 0) {
    std::string response = "ERROR" + DELIM + "LEAVE" + DELIM
      + "cannot leave #" + chatRoomName;
    send(clientFd, response.c_str(), response.size(), 0);
    return;
  }
  auto clientName = names.at(clientFd);
  chatRooms[chatRoomName].erase(clientFd);
  auto sendTo = chatRooms[chatRoomName];

  std::string response = DELIM + "LEAVE" + DELIM + chatRoomName;
  send(clientFd, response.c_str(), response.size(), 0);

  std::string msg = "INFO" + DELIM + clientName + "#" + chatRoomName
    + DELIM + "left chatroom.";
  broadcast(sendTo, clientFd, msg);
}
