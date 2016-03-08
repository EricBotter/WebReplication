#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sstream>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <sys/errno.h>

//#define PRINT_TEXT_CONN
#ifdef PRINT_TEXT_CONN
#include <iostream>
#endif

#include "Connection.h"
#include "../Utilities/Log.h"

Connection::Connection(int sockfd) : sockfd(sockfd), recvBufPos(0) { }

Connection::Connection(const string& host, uint16_t port) : recvBufPos(0) {
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

Connection::Connection(const string& destHost, uint16_t destPort, const string& sourceHost, uint16_t sourcePort) : recvBufPos(0) {
	addrinfo host_info = {};
	addrinfo* host_info_list;
	sockaddr_in sin = {};
	host_info.ai_family = AF_INET;
	host_info.ai_socktype = SOCK_STREAM;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(sourcePort);
	inet_aton(sourceHost.c_str(), &sin.sin_addr);

	int status;
	stringstream ss;
	ss << destPort;
	status = getaddrinfo(destHost.c_str(), ss.str().c_str(), &host_info, &host_info_list);
	if (status != 0) {
		Log::f("Error while DNS:");
		Log::f(gai_strerror(status));
	}

	sockfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (sockfd < 0) {
		Log::f("Unable to open socket: ");
		Log::f(strerror(errno));
	}

	status = bind(sockfd, (sockaddr*)&sin, sizeof(sin));
	if (status != 0) {
		Log::f("Error while connecting:");
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
#ifdef PRINT_TEXT_CONN
	cout << "@Sent: " << message;
#endif
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
#ifdef PRINT_TEXT_CONN
			cout << "@Received (buffered): " << out;
#endif
			return out;
		}
	}
	do {
		size = recv(sockfd, recvBuffer + recvBufPos, RECV_BUFFER_SIZE - recvBufPos, 0);
		if (size <= 0) {
			recvBufPos = 0;
			return "";
		}
		end = strstr(recvBuffer, delimiter.c_str());
		if (end == NULL) {
			out.append(recvBuffer, size + recvBufPos);
			recvBufPos = 0;
		} else {
			end += delimiter.length();
			out.append(recvBuffer, end - recvBuffer);
			recvBufPos = recvBuffer + size - end;
			memmove(recvBuffer, end, recvBufPos);
#ifdef PRINT_TEXT_CONN
			cout << "@Received: " << out;
#endif
			return out;
		}
	} while (1);
}

string Connection::receive(size_t bytes) {
	string out;
	ssize_t size;
	if (bytes <= recvBufPos) {
		out = string(recvBuffer, bytes);
		recvBufPos -= bytes;
		memmove(recvBuffer, recvBuffer + bytes, recvBufPos);
#ifdef PRINT_TEXT_CONN
		cout << "@Received (buffered): " << out;
#endif
		return out;
	}
	do {
		size = recv(sockfd, recvBuffer + recvBufPos, RECV_BUFFER_SIZE - recvBufPos, 0);
		if (size <= 0) {
			recvBufPos = 0;
			return "";
		}
		size += recvBufPos;
		recvBufPos = 0;
		if (size < bytes) {
			out.append(recvBuffer, (size_t)size);
			bytes -= size;
		} else {
			out.append(recvBuffer, bytes);
			recvBufPos = size - bytes;
			memmove(recvBuffer, recvBuffer+bytes, recvBufPos);
#ifdef PRINT_TEXT_CONN
			cout << "@Received (buffered): " << out;
#endif
			return out;
		}
	} while (1);
}

string Connection::getRemoteAddress() {
	sockaddr_in addr;
	socklen_t addr_size = sizeof(sockaddr_in);
	int res = getpeername(sockfd, (sockaddr *)&addr, &addr_size);
	char *clientip = new char[20];
	strcpy(clientip, inet_ntoa(addr.sin_addr));
	stringstream ss;
	ss << clientip << ":" << ntohs(addr.sin_port);
	delete clientip;
	return ss.str();
}
