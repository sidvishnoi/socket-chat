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

using std::cout; using std::cin; using std::endl;

int Socket();
int Connect(int sockfd, int port);
int clientChat(int sockfd);
void msgPrompt(std::string &name);

const int BUFFER_SIZE = 1024;

