#ifndef SERVER
#define SERVER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <cerrno>

using std::cout; using std::cin; using std::endl;

void printError();

int Socket();
void Bind(int sockfd, int port);
void Listen(int sockfd, int backlog);
int Accept(int sockfd);

#endif
