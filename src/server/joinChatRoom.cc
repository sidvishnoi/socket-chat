#include "./server.h"

void joinChatRoom(string chatRoomName, int clientFd, FdToName &clients, ChatRoomToFd &chatRooms) {
  
  auto chatRoom = find_if(chatRooms.begin(),
  chatRooms.end(),
  [&chatRoomName](auto const &itr) -> bool {
    return itr.first == chatRoomName;
  });

  string msg;
  if(chatRoom == chatRooms.end()) {
    msg = "Error : Chat room unavailable!!";
    send(clientFd, msg.c_str(), msg.size(), 0); 
    return;  
  }
  std::vector<int> temp = chatRooms[chatRoomName];
  for(auto it = chatRooms[chatRoomName].begin(); it != chatRooms[chatRoomName].end(); ++it) {
    if(*it == clientFd) {
      msg = "Already member!!";
      send(clientFd, msg.c_str(), msg.size(), 0);
      return; 
    }
  }
  chatRooms[chatRoomName].push_back(clientFd);
  msg = "Joined successfully!!";
  send(clientFd, msg.c_str(), msg.size(), 0);
  msg = clients[clientFd] + " joined " + chatRoomName + " !!";
  broadcastToChatRoom(clients, chatRooms, chatRoomName, clientFd, msg);
}