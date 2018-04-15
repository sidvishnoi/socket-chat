#include "./server.h"

// send welcome msg to newly connected client with currentClientFd 
void firstMsg(ChatRoomToFd &chatRooms, int currentClientFd) {
 string msg = "";
   if (!chatRooms.empty()) {
    msg = "Currently available chat-rooms !!";
    send(currentClientFd, msg.c_str(), msg.size(), 0);
    for (auto iterator = chatRooms.begin(); iterator != chatRooms.end(); ++iterator) {
      msg = iterator->first + "<" + std::to_string(iterator->second.size()) + ">";
      cout << iterator->second.size();
      send(currentClientFd, msg.c_str(), msg.size(), 0);
    } 
  }
  else {
    msg = "Currently no available chatRooms";
    send(currentClientFd, msg.c_str(), msg.size(), 0);
  }
  
  msg = "Enter /q to quit!!\n@clientName to send private msg\n.switch#chatRoomName to switch chat room"
  "\n.join#chatRoomName to join chat room\n#chatRoomName followed by msg to send msg to a particular chatRoom"
  "\n.create#chatRoomName to create new chatRoom";
  send(currentClientFd, msg.c_str(), msg.size(), 0);
}
