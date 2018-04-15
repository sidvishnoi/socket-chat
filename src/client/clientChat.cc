#include "./client.h"

int clientChat(int sockfd, const std::string &username) {
  using namespace cmd;
  char buffer[BUFFER_SIZE] = {0};
  fd_set master;
  int maxfd = sockfd + 1;
  std::string activeChatroom;
  std::set<std::string> activeChatrooms;
  bool allowMessages = false;

  while (true) {
    msgPrompt(username, activeChatroom);
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
      if (msgToSend.empty()) continue;

      auto cmdType = getCommandType(msgToSend);
      if (cmdType == NOT_CMD) {
        if (!allowMessages) {
          printMessage("ERROR" + DELIM + "JOIN" + DELIM + "You must join a chatroom first.");
          continue;
        }
        // not a command, send as message
        send(sockfd, msgToSend.c_str(), msgToSend.size(), 0);
        continue;
      }

      switch (cmdType) {
        case QUIT: return 0;
        case HELP: {
          printHelpMessage();
          continue;
        }
        case JOIN: {
          auto chatroom = split(msgToSend, " ", 2)[1].substr(1);
          std::string request = DELIM + "JOIN" + DELIM + chatroom;
          send(sockfd, request.c_str(), request.size(), 0);
          continue;
        }

        case LEAVE: {
          auto chatroom = split(msgToSend, " ", 2)[1].substr(1);
          std::string request = DELIM + "LEAVE" + DELIM + chatroom;
          send(sockfd, request.c_str(), request.size(), 0);
          continue;
        }

        case LIST_CHATROOMS: {
          std::string request = DELIM + "LIST_CHATROOMS";
          send(sockfd, request.c_str(), request.size(), 0);
          continue;
        }

        case LIST_PEOPLE: {
          if (!allowMessages) {
            printMessage("ERROR" + DELIM + "JOIN" + DELIM + "You must join a chatroom first.");
            continue;
          }
          std::string request = DELIM + "LIST_PEOPLE" + DELIM + activeChatroom;
          send(sockfd, request.c_str(), request.size(), 0);
          continue;
        }

        case INVALID:
        default:
          printMessage("ERROR" + DELIM + "CMD" + DELIM + "Invalid command.");
      }
    }

    if (FD_ISSET(sockfd, &master)) {
      memset(buffer, 0, BUFFER_SIZE);
      if (recv(sockfd, buffer, BUFFER_SIZE, 0) <= 0) {
        cout << "Server unavailable\n";
        return 0;
      }
      std::string receivedMsg(buffer);
      if (receivedMsg.find(DELIM) == 0) {
        // process response of request
        auto tokens = split(receivedMsg, DELIM, 2);
        auto &type = tokens[0], &arg = tokens[1];
        if (type == "JOIN") {
          allowMessages = true;
          activeChatroom = arg;
          activeChatrooms.insert(arg);
        }
        continue;
      }
      printMessage(receivedMsg);
    }
  }
  return 0;
}
