#include "./server.h"

string getChatroomsList(const ChatroomToFdList &chatRooms) {
  string chatRoomsList = "INFO" + DELIM + "ROOMS" + DELIM + "\n";
  for (const auto &room : chatRooms) {
    chatRoomsList += "\n" + room.first +
      " <" + std::to_string(room.second.size()) + ">";
  }
  return chatRoomsList;
}
