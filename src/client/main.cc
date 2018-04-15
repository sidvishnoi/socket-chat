#include "./client.h"

int main(int argc, char *argv[]) {
	cout << ALT_SCREEN_ON;
	const int port = (argc > 1) ? std::atoi(argv[1]) : 8000;
	StrPair credentials = getCredentials();
	cout << endl;

	const int sockfd = Socket();
	Connect(sockfd, port);

	if (!login(sockfd, credentials)) return 1;
	clientChat(sockfd, credentials.first);
	close(sockfd);
	cout << ALT_SCREEN_OFF;
	return 0;
}
