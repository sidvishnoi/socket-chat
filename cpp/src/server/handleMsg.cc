#include "./server.h"

void handleMsg(
  const int currentClientFd,
  const ChatroomToFdList &chatRooms,
  const FdToName &clients,
  string msg
) {
  auto tokens = split(msg, DELIM, 2);
  auto chatRoomName = tokens[0];
  auto text = tokens[1];
  if (chatRooms.count(chatRoomName) != 0) {
    if (chatRooms.at(chatRoomName).count(currentClientFd) != 0) {
      auto msgToSend = "MSG" + DELIM
        + clients.at(currentClientFd) + "#" + chatRoomName + DELIM
        + text;
      auto index = text.find('@');
      // send private message.
      if (index != string::npos) {
        string receiverName = text.substr(index + 1, text.substr(index + 1).find(' '));
        auto receiverItr = find_if(
          clients.begin(),
          clients.end(),
          [&receiverName](auto const &itr) -> bool {
            return itr.second == receiverName;
          });
        // Send msg to the receiver if it exist in the chatroom.
        if (receiverItr != clients.end()
          && chatRooms.at(chatRoomName).count(receiverItr->first) != 0) {
          send(receiverItr->first, msgToSend.c_str(), msgToSend.size(), 0);
          return;
        }
      }
      // If not private msg, broadcast it to the chatroom.
      broadcast(chatRooms.at(chatRoomName), currentClientFd, msgToSend);
    } else {
      string errorMsg = "ERROR" + DELIM + "SERVER" + DELIM +
        "#" + chatRoomName + " must join chat-room first.";
      send(currentClientFd, errorMsg.c_str(), errorMsg.size(), 0);
    }
  } else {
    string errorMsg = "ERROR" + DELIM + "SERVER" + DELIM +
      "#" + chatRoomName + " doesn't exist.";
    send(currentClientFd, errorMsg.c_str(), errorMsg.size(), 0);
  }
}
