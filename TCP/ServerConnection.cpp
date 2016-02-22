#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <iostream>
#include <cstring>
#include "ServerConnection.h"
#include "../Utilities/Log.h"

ServerConnection::ServerConnection(uint16_t port) {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		Log::f("Unable to open socket: ");
		Log::f(strerror(errno));
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (bind(sockfd, (sockaddr*)&server, sizeof(server)) < 0)
		Log::f(strerror(errno));
	if (listen(sockfd, 5) < 0)
		Log::f(strerror(errno));
}

Connection* ServerConnection::takeConn() {
	int newsocketfd, size = sizeof(sockaddr_in);
	sockaddr_in client;
	newsocketfd = accept(sockfd, NULL, NULL);
	if (newsocketfd < 0)
		return NULL;
	return new Connection(newsocketfd);
}
