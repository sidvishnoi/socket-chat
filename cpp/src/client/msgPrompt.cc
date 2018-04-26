#include "./client.h"

void msgPrompt(const std::string &name, const std::string &chatroom) {
  cout << "\r"
    << color::green << name
    << color::reset << "#" << color::magenta << chatroom
    << color::green << " ➜ "
    << color::reset
    << std::flush;
}
