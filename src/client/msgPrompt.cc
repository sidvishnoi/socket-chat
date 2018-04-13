#include "./client.h"

void msgPrompt(const std::string &name) {
  cout << "\r" << color::green << name << "➜ "
    << color::reset << std::flush;
}
