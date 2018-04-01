/**
 * Server for chat application
 * Usage:
 *  $ ./server [PORT=8000] [BACKLOG=5]
 */

#include <iostream>
#include "./server.h"
#include "./db/db.h"

using std::cout; using std::endl;

int main(int argc, char *argv[]) {
  User user("sid", "pass", "127.0.0.1", 7898, 4);
  std::string u1 = user.serialize();
  cout << u1  << endl;
  User user2(u1);
  cout << user2.serialize() << endl;

  return 0;
  const int port = (argc > 1) ? std::atoi(argv[1]) : 8000;
  const int backlog = (argc > 2) ? std::atoi(argv[2]) : 5;

  const int sockfd = Socket();
  Bind(sockfd, port);
  Listen(sockfd, backlog);
  Accept(sockfd);

  close(sockfd);
  return errno;
}
