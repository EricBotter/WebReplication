#include <sys/unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include "Connection.h"

Connection::Connection(int sockfd) : sockfd(sockfd), recvBufPos(0) { }

Connection::Connection(string host, int port) {
	//TODO: client connection
}

Connection::~Connection() {
	close(sockfd);
}

int Connection::sendStr(string message) const {
	return send(sockfd, message.c_str(), message.length(), 0);
}

string Connection::receive(string delimiter) {
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
