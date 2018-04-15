#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

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
#include <map>
#include <vector>
#include <algorithm>
#include <vector>
#include "../db/database.h"
#include "../db/User.h"

using std::cout; using std::cin; using std::endl;
using std::string;
using std::vector;
using FdToName = std::map<int, string>;
using ChatRoomToFd = std::map<string, std::vector<int>>;

void printError();

int Socket();
void Bind(int sockfd, int port);
void Listen(int sockfd, int backlog);
int Accept(int sockfd);

string login(Database<User> &db, const string &uname, const string &pass);
bool logout(Database<User> &db, const string &uname);

int serverChat(int sockfd);
void firstMsg(ChatRoomToFd &chatRooms, int currentClientFd);
std::string getPeerName(const int sockfd);
void broadcast(FdToName &clients, int currentClientFd, const string &msg);
void broadcastToChatRoom(FdToName &clients,  ChatRoomToFd chatRooms, const string chatRoomName, int currentClientFd, const string &msg);
void privateChat(FdToName &clients, int currentClientFd, const string &msg, string clientName);
void switchChatRoom(string chatRoomName, int clientFd, ChatRoomToFd &chatRooms);
void joinChatRoom(string chatRoomName, int clientFd, FdToName &clients, ChatRoomToFd &chatRooms);
void createChatRoom(string chatRoomName, int clientFd, FdToName &clients, ChatRoomToFd &chatRooms); 

const int BUFFER_SIZE = 1024;
const string DELIM("$$$");
const string welcomeMsg("Welcome client");

vector<string> split(const string &str, const string &delim);

#endif  // SRC_SERVER_SERVER_H_
