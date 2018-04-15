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
using FdList = vector<int>;
using FdToName = std::map<int, string>;
using ChatroomToFdList = std::map<string, FdList>;

namespace cmd {
  enum Commands {
    NOT_CMD,
    INVALID,
    JOIN,
    LEAVE,
    LIST_CHATROOMS,
    LIST_PEOPLE
  };
}

void printError();

int Socket();
void Bind(int sockfd, int port);
void Listen(int sockfd, int backlog);
int Accept(int sockfd);

string login(Database<User> &db, const string &uname, const string &pass);
bool logout(Database<User> &db, const string &uname);

int serverChat(int sockfd);
cmd::Commands getMessageType(const std::string &);
std::string getChatroomsList(const ChatroomToFdList &chatRooms);
std::string getPeopleList(const std::string &chatRoomName, const FdToName &names, const ChatroomToFdList &chatRooms);
std::string getPeerName(const int sockfd);
void broadcast(const FdToName &names, const FdList &clients, const int currentClientFd, const string &msg);
void privateChat(FdToName &clients, int currentClientFd, const string &msg, string clientName);
void switchChatRoom(string chatRoomName, int clientFd, ChatroomToFdList &chatRooms);
void joinChatRoom(const std::string chatRoomName, const int clientFd, FdToName &clients, ChatroomToFdList &chatRooms);

const int BUFFER_SIZE = 1024;
const string DELIM("$$$");
const string welcomeMsg("Welcome client");

vector<string> split(const string &str, const string &delim, const int limit);

#endif  // SRC_SERVER_SERVER_H_
