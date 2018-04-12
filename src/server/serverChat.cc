#include "./server.h"

int serverChat(int sockfd) {
  int buffer_size = 1024, client[5] = {0,0,0,0,0}, max_client = 5, sock, max = sockfd;
  char buffer[buffer_size];
  fd_set master;

  while(true) {
    FD_ZERO(&master);
    FD_SET(sockfd, &master);
    for(int i = 0; i < max_client; i++) {
      sock = client[i];
      if(sock > 0)
        FD_SET(sock, &master);
      if(sock > max)
        max = sock;
    }
    select(max+1, &master, NULL,NULL,NULL);

    if(FD_ISSET(sockfd, &master)) {
      int new_client = Accept(sockfd);
      cout<<"Accepted new connection\n";
      if(new_client > 0 && errno != EINTR) {
        bool spaceFound = false;
        // FD_SET(new_client, &master);
        for(int i = 0; i < max_client; i++) {
          if(client[i] <= 0) {
            client[i] = new_client;
            cout << new_client << "connected to server" << endl;
            char msg[1024] = "Welcome client";
            send(new_client, msg, sizeof(msg),0);  
            spaceFound = true;
            break;
          }
        }
        if(!spaceFound) {
          cout << "No space in connection list\n"; 
          close(new_client);
          continue;
        }
      }
    }
    else {
      for(int i = 0; i < max_client; i++) {
        int current_client = client[i];
        if(current_client > 0 && FD_ISSET(current_client, &master)) {
          memset(buffer, 0 , buffer_size);
          if(recv(current_client, buffer, sizeof(buffer), 0) <= 0) {
            FD_CLR(current_client, &master);
            close(current_client);
            client[i] = -1;
            cout << current_client << " is offline" << endl;
            char msg[1024] = "client is offline";
            for(int j = 0; j < max_client; j++) {
              if (client[j] > 0 && client[j] != current_client)
                send(client[j], msg, sizeof(msg), 0);
            }
          }
          else {
            for(int j = 0; j < max_client; j++) {
              if(client[j] > 0 && client[j] != current_client) {
                send(client[j], buffer, sizeof(buffer), 0);
              }
            }
          }
          cout << buffer << endl;   
        }
      }
    }
  }
  return 0;
}