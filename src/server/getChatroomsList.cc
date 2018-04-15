#include "./server.h"

string getChatroomsList(const ChatRoomToFd &chatRooms) {
 string chatRoomsList = "INFO" + DELIM + "ROOMS" + DELIM + "\n";
  for (const auto &room : chatRooms) {
    chatRoomsList += room.first + " <" + std::to_string(room.second.size()) + ">\n";
  }
  return chatRoomsList;
}
