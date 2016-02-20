#include <sys/socket.h>
#include <netinet/in.h>
#include "ServerConnection.h"
#include "../Log.h"

ServerConnection::ServerConnection(uint16_t port) {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		Log::f("Unable to open socket");
		//TODO: error handling
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	bind(sockfd, (sockaddr*)&server, sizeof(server));
}

Connection* ServerConnection::accept() {
	int newsocketfd, size = sizeof(sockaddr_in);
	sockaddr_in client;
	newsocketfd = accept(sockfd, (sockaddr*)&client, &size);
	//TODO: error handling
	return new Connection(newsocketfd);
}
