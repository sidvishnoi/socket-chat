#include "./client.h"

int main(int argc, char *argv[]) {
	cout << ALT_SCREEN_ON;
	const std::string host = (argc > 1) ? argv[1] :  "127.0.0.1";
	const int port = (argc > 2) ? std::atoi(argv[2]) : 8000;
	StrPair credentials = getCredentials();

	try {
		const int sockfd = Socket();
		Connect(sockfd, host, port);
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
