#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <iostream>
#include <stdexcept>
#include <cerrno>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "../db/database.h"
#include "../db/User.h"

using std::cout; using std::cin; using std::endl;
using std::string;
using std::vector;
using FdList = std::set<int>;
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

int Socket();
void Bind(int sockfd, int port);
void Listen(int sockfd, int backlog);
int Accept(int sockfd);

// print the errno codes
void printError();

// login a user
string login(Database<User> &db, const string &uname, const string &pass);

// logout a user
bool logout(Database<User> &db, const string &uname);

// main chat function
int serverChat(int sockfd);

// add a client to chat system (with login)
bool addClient(const int currentClientFd, fd_set *master, const std::string &credentials, Database<User> &db, FdToName &names);

// get the msg type - regular msg or some command
cmd::Commands getMessageType(const std::string &);

// response for `/list rooms`
std::string getChatroomsList(const ChatroomToFdList &chatRooms);

// response for `/list people`
std::string getPeopleList(const std::string &chatRoomName, const FdToName &names, const ChatroomToFdList &chatRooms);

// broadcast msg to all `clients` except currentClientFd (sender)
void broadcast(const FdToName &names, const FdList &clients, const int currentClientFd, const string &msg);

// handle private messages
void privateChat(FdToName &clients, int currentClientFd, const string &msg, string clientName);

// add a user to a chatroom `/join #chatroom`
void joinChatRoom(const std::string chatRoomName, const int clientFd, FdToName &clients, ChatroomToFdList &chatRooms);

// remove a user from a chatroom `/leave #chatroom`
void leaveChatRoom(const std::string chatRoomName, const int clientFd, const FdToName &names, ChatroomToFdList &chatRooms);

// get the `ip:port` for a given client
std::string getPeerName(const int sockfd);

const int BUFFER_SIZE = 1024;
const string DELIM("$$$");

vector<string> split(const string &str, const string &delim, const int limit);

#endif  // SRC_SERVER_SERVER_H_
