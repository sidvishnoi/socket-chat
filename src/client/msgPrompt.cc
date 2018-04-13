#include "./client.h"

void msgPrompt(std::string &name) {
  cout << "\r" << color::green << name << "➜ "
    << color::reset << std::flush;
}
