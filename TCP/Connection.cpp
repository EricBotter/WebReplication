#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sstream>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <sys/errno.h>

//#define PRINT_TEXT_CONN

#include "Connection.h"
#include "../Utilities/Log.h"

Connection::Connection(int sockfd) : sockfd(sockfd), recvBufPos(0) {
	Log::t("Creating Connection from existing socket " + to_string(sockfd));
}

Connection::Connection(const string& host, uint16_t port) : recvBufPos(0) {
	addrinfo host_info;
	addrinfo* host_info_list;

	memset(&host_info, 0, sizeof(host_info));
	host_info.ai_family = AF_INET;
	host_info.ai_socktype = SOCK_STREAM;

	Log::t("Connecting to " + host + " on port " + to_string(port));

	int status;
	status = getaddrinfo(host.c_str(), to_string(port).c_str(), &host_info, &host_info_list);
	if (status != 0) {
		errorCode = status == EAI_SYSTEM ? errno : -1;
		Log::f("Error while DNS:");
		Log::f(gai_strerror(status));
		return;
	}

	sockfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (sockfd < 0) {
		errorCode = errno;
		Log::f("Unable to open socket: ");
		Log::f(strerror(errno));
		return;
	}

	status = connect(sockfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
	if (status != 0) {
		errorCode = errno;
		Log::f("Error while connecting:");
		Log::f(strerror(errno));
		return;
	}

	Log::t("Connection to host " + host + ':' + to_string(port) + " started on socket " + to_string(sockfd));
}

Connection::Connection(const string& destHost, uint16_t destPort, const string& sourceHost, uint16_t sourcePort)
		: recvBufPos(0) {
	addrinfo host_info = {};
	addrinfo* host_info_list;
	sockaddr_in sin = {};
	host_info.ai_family = AF_INET;
	host_info.ai_socktype = SOCK_STREAM;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(sourcePort);
	inet_aton(sourceHost.c_str(), &sin.sin_addr);

	Log::t("Connecting to " + destHost + " on port " + to_string(destPort) + ", from source address " + sourceHost +
		   ':' + to_string(sourcePort));

	int status;
	status = getaddrinfo(destHost.c_str(), to_string(destPort).c_str(), &host_info, &host_info_list);
	if (status != 0) {
		errorCode = status == EAI_SYSTEM ? errno : -1;
		Log::f("Error while DNS:");
		Log::f(gai_strerror(status));
		return;
	}

	sockfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (sockfd < 0) {
		errorCode = errno;
		Log::f("Unable to open socket: ");
		Log::f(strerror(errno));
		return;
	}

	status = bind(sockfd, (sockaddr*)&sin, sizeof(sin));
	if (status != 0) {
		errorCode = errno;
		Log::f("Error while binding to specified address:");
		Log::f(strerror(errno));
		return;
	}

	status = connect(sockfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
	if (status != 0) {
		errorCode = errno;
		Log::f("Error while connecting:");
		Log::f(strerror(errno));
		return;
	}

	Log::t("Connection established on socket " + to_string(sockfd));
}

Connection::~Connection() {
	Log::t("Closing connection " + to_string(sockfd));
	close(sockfd);
}

int Connection::sendStr(const string& message) const {
	int bytes = send(sockfd, message.c_str(), message.length(), 0);
#ifdef PRINT_TEXT_CONN
	Log::t("Connection " + to_string(sockfd) + " sent:\n" + message);
#else
	Log::t("Connection " + to_string(sockfd) + " sent " + to_string(bytes) + '/' + to_string(message.length()) + " bytes");
#endif
	return bytes;
}

string Connection::receive(const string& delimiter) {
	string out;
	size_t index = 0;
	ssize_t size;
	if (recvBufPos != 0) {
		out.append(recvBuffer, recvBufPos);
		index = out.find(delimiter);
		if (index != out.npos) {
			index += delimiter.length();
			recvBufPos = out.length() - index;
			memcpy(recvBuffer, out.c_str() + index, recvBufPos);
			out.erase(index);
#ifdef PRINT_TEXT_CONN
			Log::t("Connection " + to_string(sockfd) + " received (buffered):\n" + out);
#else
			Log::t("Connection " + to_string(sockfd) + " received " + to_string(out.length()) + " bytes (buffered)");
#endif
			return out;
		}
	}
	recvBufPos = 0;
	for (;;) {
		size = recv(sockfd, recvBuffer, RECV_BUFFER_SIZE, 0);
		if (size <= 0) {
			return "";
		}
		out.append(recvBuffer, (size_t)size);
		index = out.find(delimiter, out.length() < size + delimiter.length() ? 0 : out.length() - size - delimiter.length());
		if (index != out.npos) {
			index += delimiter.length();
			recvBufPos = out.length() - index;
			memcpy(recvBuffer, out.c_str() + index, recvBufPos);
			out.erase(index);
#ifdef PRINT_TEXT_CONN
			Log::t("Connection " + to_string(sockfd) + " received:\n" + out);
#else
			Log::t("Connection " + to_string(sockfd) + " received " + to_string(out.length()) + " bytes");
#endif
			return out;
		}
	}
}

string Connection::receive(size_t bytes) {
	string out;
	ssize_t size;
	if (bytes <= recvBufPos) {
		out = string(recvBuffer, bytes);
		recvBufPos -= bytes;
		memmove(recvBuffer, recvBuffer + bytes, recvBufPos);
#ifdef PRINT_TEXT_CONN
		Log::t("Connection " + to_string(sockfd) + " received (buffered):\n" + out);
#else
		Log::t("Connection " + to_string(sockfd) + " received " + to_string(out.length()) + " bytes (buffered)");
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
			memmove(recvBuffer, recvBuffer + bytes, recvBufPos);
#ifdef PRINT_TEXT_CONN
			Log::t("Connection " + to_string(sockfd) + " received:\n" + out);
#else
			Log::t("Connection " + to_string(sockfd) + " received " + to_string(out.length()) + " bytes");
#endif
			return out;
		}
	} while (1);
}

//Deprecated
string Connection::getRemoteAddress() {
	sockaddr_in addr;
	socklen_t addr_size = sizeof(sockaddr_in);
	int res = getpeername(sockfd, (sockaddr*)&addr, &addr_size);
	char* clientip = new char[20];
	strcpy(clientip, inet_ntoa(addr.sin_addr));
	stringstream ss;
	ss << clientip << ":" << ntohs(addr.sin_port);
	delete clientip;
	return ss.str();
}

int Connection::error() {
	return errorCode;
}

