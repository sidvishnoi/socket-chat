#include "./server.h"

void createChatRoom(string chatRoomName, int clientFd, FdToName &clients, ChatRoomToFd &chatRooms) {
  
  auto chatRoom = find_if(chatRooms.begin(),
  chatRooms.end(),
  [&chatRoomName](auto const &itr) -> bool {
    return itr.first == chatRoomName;
  });

  string msg;
  if (chatRoom != chatRooms.end()) {
    msg = "Chat room already exist!!";
    send(clientFd, msg.c_str(), msg.size(), 0); 
    return;
  }
  std::vector<int> members;
  members.push_back(clientFd);
  chatRooms[chatRoomName] = members;
  msg = "Chat room created successfully!!";
  send(clientFd, msg.c_str(), msg.size(), 0);
  msg = "New chat room " + chatRoomName + " created!!";
  broadcast(clients, clientFd, msg);
}