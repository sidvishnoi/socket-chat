#include "./client.h"

int clientChat(int sockfd, const std::string &username) {
  char buffer[BUFFER_SIZE] = {0};
  fd_set master;
  int maxfd = sockfd + 1;

  while (true) {
    msgPrompt(username);
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
      std::string msgToSend;
      getline(cin, msgToSend);
      if (cin.fail() || cin.eof() || msgToSend == "/q") {
        cout << "Connection closed!!\n";
        return 0;
      }
      cout << chatRoom << " dsi" << endl;
      msgToSend = "$" +   chatRoom + " " + msgToSend;
      //cout << msgToSend << endl;
      //cout << msgToSend.size() << endl;
      send(sockfd, msgToSend.c_str(), msgToSend.size(), 0);
    }

    if (FD_ISSET(sockfd, &master)) {
      memset(buffer, 0, BUFFER_SIZE);
      if (recv(sockfd, buffer, BUFFER_SIZE, 0) <= 0) {
        cout << "Server unavailable\n";
        return 0;
      }
      std::string receivedMsg(buffer);
      printMessage(receivedMsg);
    }
  }
  return 0;
}
