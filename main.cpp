#include <sys/socket.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include "Log.h"

using namespace std;

#define SOCKET_PORT 8000

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		Log::f("Unable to open socket");
		return 1;
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SOCKET_PORT);

	if (bind(sockfd, (sockaddr*)&server, sizeof(server)) < 0) {
		Log::f("Unable to bind socket");
		return 2;
	}

	listen(sockfd, 5);
	Log::d("Server started. Waiting for connection...");

	int newsockfd, c = sizeof(sockaddr_in);
	sockaddr_in client;
	while ((newsockfd = accept(sockfd, (sockaddr*)&client, &c)) >= 0) {

	}

	if (newsockfd < 0) {
		Log::f("Unable to accept other incoming connections");
		return 2;
	}

	close(sockfd);

	return 0;
}