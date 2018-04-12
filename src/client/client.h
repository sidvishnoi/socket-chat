#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <cerrno>
#include <sys/select.h>
#include <sys/time.h>

using std::cout; using std::cin; using std::endl;

int Socket();
int Connect(int sockfd, int port);
int clientChat(int sockfd);