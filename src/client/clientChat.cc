#include "./client.h"

int clientChat(int sockfd) {
  int buffer_size = 1024;
  char buffer[buffer_size] = {0};

  if (recv(sockfd, buffer, buffer_size, 0) <= 0) {
    cout << "error in receiving message" << endl;
    return 0;
  } else {
    cout << "MSG -> " << buffer << endl;
  }
  cout << "Enter quit for exit" << endl;

  fd_set master;
  int maxfd = sockfd + 1;

  while (true) {
    FD_ZERO(&master);
    FD_SET(sockfd, &master);
    FD_SET(STDIN_FILENO, &master);
    int sockCount = select(maxfd, &master, NULL, NULL, NULL);

    if (sockCount == -1) {
      throw std::runtime_error("socket count = -1");
    } else if (sockCount == 0) {
      cout << "select Timeout." << endl;
      continue;
    }

    if (FD_ISSET(STDIN_FILENO, &master)) {
      cin.getline(buffer, buffer_size);
      if (cin.fail() || cin.eof() || strcmp(buffer, "quit") == 0) {
        cout << "Connection closed!!\n";
        return 0;
      }
      send(sockfd, buffer, buffer_size, 0);
      if (strcmp(buffer, "quit") == 0)
        return 0;
      } else if (FD_ISSET(sockfd, &master)) {
        memset(buffer, 0 , buffer_size);
        if (recv(sockfd, buffer, buffer_size, 0) <= 0) {
          cout << "Server unavailable\n";
          return 0;
        }
        cout << "Msg -> " << buffer << endl;
      }
    }
  return 0;
}
