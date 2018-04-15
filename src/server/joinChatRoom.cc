#include "./server.h"

void joinChatRoom(const std::string chatRoomName, const int clientFd, FdToName &names, ChatroomToFdList &chatRooms) {
  auto chatRoomItr = find_if(
    chatRooms.begin(),
    chatRooms.end(),
    [&chatRoomName](auto const &itr) -> bool {
      return itr.first == chatRoomName;
    }
  );

  bool exists = chatRoomItr != chatRooms.end();
  if(!exists) {
    chatRooms[chatRoomName] = FdList{clientFd};
  } else {
    chatRooms[chatRoomName].insert(clientFd);
  }

  std::string response = DELIM + "JOIN" + DELIM + chatRoomName;
  send(clientFd, response.c_str(), response.size(), 0);

  // let members of chatroom know of new user
  if (exists) {
    std::string msg = "INFO" + DELIM + names[clientFd] + DELIM + "joined " + chatRoomName;
    broadcast(names, chatRooms.at(chatRoomName), clientFd, msg);
  }
}
