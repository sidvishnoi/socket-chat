#ifndef SRC_CLIENT_CLIENT_H_
#define SRC_CLIENT_CLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <cerrno>
#include <string>
#include <vector>
#include <set>

const std::string ALT_SCREEN_ON("\033[2J\033[1;1H");
const std::string ALT_SCREEN_OFF("\033[2J\033[?47l\0338");

using std::cout; using std::cin; using std::endl;
using StrPair = std::pair<std::string, std::string>;

namespace cmd {
  enum Commands {
    NOT_CMD,
    INVALID,
    QUIT,
    HELP,
    JOIN,
    LEAVE,
    LIST_CHATROOMS,
    LIST_PEOPLE
  };
}

int Socket();
int Connect(int sockfd, const std::string &host, int port);
StrPair getCredentials();
bool login(const int sockfd, const StrPair& credentials);
int clientChat(int sockfd, const std::string &username);
void msgPrompt(const std::string &name, const std::string &chatroom);
void printMessage(const std::string &msg);
void printHelpMessage();

cmd::Commands getCommandType(const std::string &);

namespace color {
  const std::string red("\033[0;31m");
  const std::string green("\033[1;32m");
  const std::string yellow("\033[1;33m");
  const std::string cyan("\033[0;36m");
  const std::string magenta("\033[0;35m");
  const std::string reset("\033[0m");
}

std::vector<std::string> split(const std::string &str, const std::string &delim, const int limit);

const int BUFFER_SIZE = 1024;
const std::string DELIM("$$$");

#endif  // SRC_CLIENT_CLIENT_H_
