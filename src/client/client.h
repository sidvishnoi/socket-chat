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

using std::cout; using std::cin; using std::endl;
using StrPair = std::pair<std::string, std::string>;

int Socket();
int Connect(int sockfd, int port);
StrPair getCredentials();
bool login(const int sockfd, const StrPair& credentials);
int clientChat(int sockfd, const std::string &username);
void msgPrompt(const std::string &name);
void printMessage(const std::string &msg);
void printHelpMessage();

namespace color {
  const std::string red("\033[0;31m");
  const std::string green("\033[1;32m");
  const std::string yellow("\033[1;33m");
  const std::string cyan("\033[0;36m");
  const std::string magenta("\033[0;35m");
  const std::string reset("\033[0m");
}

std::vector<std::string> split(const std::string &str, const std::string &delim);

const int BUFFER_SIZE = 1024;
const std::string DELIM("$$$");

#endif  // SRC_CLIENT_CLIENT_H_
