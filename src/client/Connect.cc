#include "./client.h"

int Connect(int sockfd, int port) {

	if(sockfd < 0) {
		cout << "Error in socket creation" << endl;
		return -1;
	}
	
	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	int x = inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr));

	if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		cout << "Error in making connection" << endl;
	}
	else
		cout << "Connection established" << endl;
	return 0; 
}
