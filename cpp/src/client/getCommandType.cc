#include "./client.h"

cmd::Commands getCommandType(const std::string &msg) {
  using namespace cmd;
  if (msg[0] != '/') return NOT_CMD;
  if (msg == "/q") return QUIT;
  if (msg == "/help") return HELP;

  std::vector<std::string> tokens;
  try {
    tokens = split(msg, " ", 2);
  } catch (...) {
    return INVALID;
  }
  if (tokens.size() < 2) return INVALID;

  if (tokens[0] == "/join" && tokens[1][0] == '#') return JOIN;
  if (tokens[0] == "/leave" && tokens[1][0] == '#') return LEAVE;
  if (tokens[0] == "/list" && tokens[1] == "people") return LIST_PEOPLE;
  if (tokens[0] == "/list" && tokens[1] == "rooms") return LIST_CHATROOMS;
  return INVALID;
}
