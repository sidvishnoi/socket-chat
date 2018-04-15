#include "./client.h"

int main(int argc, char *argv[]) {
	cout << ALT_SCREEN_ON;
	const int port = (argc > 1) ? std::atoi(argv[1]) : 8000;
	StrPair credentials = getCredentials();

	try {
		const int sockfd = Socket();
		Connect(sockfd, port);
		login(sockfd, credentials);
		clientChat(sockfd, credentials.first);
		close(sockfd);
	} catch (std::exception &e) {
		cout << color::red << "ERROR: " << e.what() << color::reset << endl;
		return errno;
	}
	cout << ALT_SCREEN_OFF;
	return 0;
}
