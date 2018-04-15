#include "./server.h"

void switchChatRoom(string chatRoomName, int clientFd, ChatRoomToFd &chatRooms) {
  
  auto switchedChatRoom = find_if(chatRooms.begin(),
  chatRooms.end(),
  [&chatRoomName](auto const &itr) -> bool {
    return itr.first == chatRoomName;
  });

  string msg;
  if (switchedChatRoom != chatRooms.end()) {
    std::vector<int> temp = chatRooms[chatRoomName];
    for(auto it = temp.begin(); it != temp.end(); ++it) {
      if(*it == clientFd) {
        msg = "Switched succesfully!!";
        send(clientFd, msg.c_str(), msg.size(), 0);
        return; 
      }
    }
  }
  msg = "Error : Switch unsuccesfully!!";
  send(clientFd, msg.c_str(), msg.size(), 0); 
  return;
}