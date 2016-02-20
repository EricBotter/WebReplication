#include <sys/unistd.h>
#include "Connection.h"

#define SEND_BUFFER_SIZE 2048
#define RECV_BUFFER_SIZE 2048

Connection::Connection(int sockfd) {
	this->sockfd = sockfd;
	sendBufPos = recvBufPos = 0;
	sendBuffer = new char[SEND_BUFFER_SIZE];
	recvBuffer = new char[RECV_BUFFER_SIZE];
}

Connection::Connection(string host, int port) {
	//TODO: client connection
}

Connection::~Connection() {
	close(sockfd);
	delete[] sendBuffer;
	delete[] recvBuffer;
}

int Connection::send(string message) {
	return 0;
}

string Connection::receive(string delimiter) {
	return "";
}

string Connection::receive(int bytes) {
	return "";
}
