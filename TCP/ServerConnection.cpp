#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <iostream>
#include <cstring>
#ifdef __APPLE__
#include <unistd.h>
#else
#include <sys/unistd.h>
#endif
#include "ServerConnection.h"
#include "../Utilities/Log.h"

ServerConnection::ServerConnection(uint16_t port) {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		Log::f("Unable to open listening socket: " + string(strerror(errno)));
		lastError = errno;
		return;
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (::bind(sockfd, (sockaddr*)&server, sizeof(server)) < 0) {
		Log::f("Unable to bind socket " + to_string(sockfd) + ": " + string(strerror(errno)));
		lastError = errno;
		return;
	}
	if (listen(sockfd, 5) < 0) {
		Log::f("Unable to listen on socket " + to_string(sockfd) + ": " + string(strerror(errno)));
		lastError = errno;
		return;
	}
}

Connection* ServerConnection::takeConn() {
	int newsocketfd, size = sizeof(sockaddr_in);
	sockaddr_in client;
	newsocketfd = accept(sockfd, NULL, NULL);
	if (newsocketfd < 0)
		return NULL;
	Log::t("Accepted connection on socket " + to_string(newsocketfd));
	return new Connection(newsocketfd);
}

ServerConnection::~ServerConnection() {
	Log::t("Closing server socket " + to_string(sockfd));
	close(sockfd);
}

int ServerConnection::error() {
	int temp = lastError;
	lastError = 0;
	return temp;
}
