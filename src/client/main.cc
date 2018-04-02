#include "./client.h"

int main(int argc, char *argv[]) {

	const int port = (argc > 1) ? atoi(argv[1]) : 8000;
	const int sockfd = Socket();
	Connect(sockfd, port);
	close(sockfd);
	return 0;
}