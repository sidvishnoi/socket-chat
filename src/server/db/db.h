#ifndef SRC_SERVER_DB_DB_H_
#define SRC_SERVER_DB_DB_H_

#include <string>
#include <sstream>
#include <map>

class User {
  std::string username;
  std::string password;
  std::string host;
  int port;
  int sockfd;
  const int maxLength = 128;

 public:
  User(const std::string &serializedString);
  User(const std::string &un,
    const std::string &pw,
    const std::string &h,
    const int p,
    const int sock)
    : username(un), password(pw), host(h), port(p), sockfd(sock) { }

  std::string serialize();

  std::string getName() const { return username; }
  void setName(const std::string &newName) { username = newName; }

  std::string getPassword() const { return password; }
  void setPassword(const std::string& newPass) { password = newPass; }

  std::string getHost() const { return host; }
  void setHost(const std::string &newHost) { host = newHost; }

  int getPort() const { return port; }
  void setPort(const int newPort) { port = newPort; }

  int getSockfd() const { return sockfd; }
  void setSockfd(const int newSock) { sockfd = newSock; }
};

enum statusCode {
  SUCCESS,
  WARNING,
  FAILED
};

template <class T>
class Database {
 public:
  explicit Database(std::string dir): dbDir(dir) {}

  statusCode add(T&);
  T get(const std::string& key, const std::string &value) const;
  statusCode update(T &oldUser, T &newUser);
  statusCode remove(T &);

 private:
  std::string dbDir;
  const int hashLength = 2;
};

#endif  //  SRC_SERVER_DB_DB_H_
