#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <sys/errno.h>
#include <netdb.h>
#include <sstream>
#include "Connection.h"
#include "../Utilities/Log.h"

Connection::Connection(int sockfd) : sockfd(sockfd), recvBufPos(0) { }

Connection::Connection(const string& host, uint16_t port) {
	addrinfo host_info;
	addrinfo* host_info_list;

	memset(&host_info, 0, sizeof(host_info));
	host_info.ai_family = AF_INET;
	host_info.ai_socktype = SOCK_STREAM;

	int status;
	stringstream ss;
	ss << port;
	status = getaddrinfo(host.c_str(), ss.str().c_str(), &host_info, &host_info_list);
	if (status != 0) {
		Log::f("Error while DNS:");
		Log::f(gai_strerror(status));
	}

	sockfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (sockfd < 0) {
		Log::f("Unable to open socket: ");
		Log::f(strerror(errno));
	}

	status = connect(sockfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
	if (status != 0) {
		Log::f("Error while connecting:");
		Log::f(strerror(errno));
	}
}

Connection::~Connection() {
	close(sockfd);
}

int Connection::sendStr(const string& message) const {
	return send(sockfd, message.c_str(), message.length(), 0);
}

string Connection::receive(const string& delimiter) {
	string out;
	ssize_t size;
	char* end = strstr(recvBuffer, delimiter.c_str());
	if (end != NULL) {
		end += delimiter.length();
		if (end - recvBuffer < recvBufPos) {
			out = string(recvBuffer, end - recvBuffer);
			recvBufPos = end - recvBuffer;
			memmove(recvBuffer, end, recvBufPos);
			return out;
		}
	}
	do {
		size = recv(sockfd, recvBuffer + recvBufPos, RECV_BUFFER_SIZE - recvBufPos, 0);
		end = strstr(recvBuffer, delimiter.c_str());
		if (end == NULL) {
			out.append(recvBuffer, size + recvBufPos);
			recvBufPos = 0;
		} else {
			end += delimiter.length();
			out.append(recvBuffer, end - recvBuffer);
			recvBufPos = recvBuffer + size - end;
			memmove(recvBuffer, end, recvBufPos);
			return out;
		}
	} while (1);
}

string Connection::receive(size_t bytes) {
	string out;
	ssize_t size;
	if (bytes < recvBufPos) {
		out = string(recvBuffer, bytes);
		recvBufPos -= bytes;
		memmove(recvBuffer, recvBuffer + bytes, recvBufPos);
		return out;
	}
	do {
		size = recv(sockfd, recvBuffer + recvBufPos, RECV_BUFFER_SIZE - recvBufPos, 0);
		size += recvBufPos;
		recvBufPos = 0;
		if (size < bytes) {
			out.append(recvBuffer, (size_t)size);
			bytes -= size;
		} else {
			out.append(recvBuffer, bytes);
			recvBufPos = size - bytes;
			memmove(recvBuffer, recvBuffer+bytes, recvBufPos);
			return out;
		}
	} while (1);
}
