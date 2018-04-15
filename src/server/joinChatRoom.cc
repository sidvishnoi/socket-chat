#include "./server.h"

void joinChatRoom(const std::string chatRoomName, const int clientFd, FdToName &clients, ChatRoomToFd &chatRooms) {
  auto chatRoom = find_if(
    chatRooms.begin(),
    chatRooms.end(),
    [&chatRoomName](auto const &itr) -> bool {
      return itr.first == chatRoomName;
    }
  );

  bool exists = chatRoom != chatRooms.end();
  if(!exists) {
    chatRooms[chatRoomName] = std::vector<int> {clientFd};
  } else {
    chatRooms[chatRoomName].push_back(clientFd);
  }
  std::string response = DELIM + "JOIN" + DELIM + chatRoomName;
  send(clientFd, response.c_str(), response.size(), 0);
  if (exists) {
    std::string msg = "INFO" + DELIM + clients[clientFd] + DELIM + "joined " + chatRoomName;
    broadcastToChatRoom(clients, chatRooms, chatRoomName, clientFd, msg);
  }
}
