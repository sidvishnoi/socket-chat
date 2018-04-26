#include "./client.h"

void printHelpMessage() {
  cout << color::yellow << "/help"
    << color::magenta << "\n  ➜ " << color::reset
    << "print this help message." << endl;
  cout << color::yellow << "/q"
    << color::magenta << "\n  ➜ " << color::reset
    << "quit." << endl;
  cout << color::yellow << "@person YOUR MESSAGE"
    << color::magenta << "\n  ➜ " << color::reset
    << "send a private msg to <person> in current chatroom." << endl;
  cout << color::yellow << "/list people"
    << color::magenta << "\n  ➜ " << color::reset
    << "list people in current chatroom." << endl;
  cout << color::yellow << "/list rooms"
    << color::magenta << "\n  ➜ " << color::reset
    << "list all chatrooms." << endl;
  cout << color::yellow << "/join #chatRoomName"
    << color::magenta << "\n  ➜ " << color::reset
    << "switch/join/create a chat room." << endl;
  cout  << color::yellow << "#chatroomName YOUR MESSAGE"
    << color::magenta << "\n  ➜ " << color::reset
    << "send message to particular chatroom. You can use @person along with this." << endl;
  cout << endl;
}
