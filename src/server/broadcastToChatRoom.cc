#include "./server.h"

// send msg to all clients of a given chatroom accept currentClientFD
void broadcastToChatRoom(FdToName &clients,  ChatRoomToFd chatRooms, const string chatRoomName, int currentClientFd, const string &msg) {
  auto msgToSendIn = find_if(chatRooms.begin(),
  chatRooms.end(),
  [&chatRoomName](auto const &itr) -> bool {
    return itr.first == chatRoomName;
  });
  if (msgToSendIn != chatRooms.end()) {
    std::vector<int> sendTo = chatRooms[chatRoomName];
    for (auto itr = sendTo.begin(); itr != sendTo.end(); ++itr) {
      if (*itr != currentClientFd) {
        send(*itr, msg.c_str(), msg.size(), 0);
      }
    }
    return;
  }
  broadcast(clients, currentClientFd, msg);
}
