#ifndef THESIS_CONNECTION_H
#define THESIS_CONNECTION_H

#include <string>

using namespace std;

class Connection {
private:
	int sockfd;
	int sendBufPos, recvBufPos;
	char* sendBuffer;
	char* recvBuffer;
public:
	//To be used by servers
	Connection(int);
	//To be used by clients
	Connection(string, int);
	~Connection();
	int send(string);
	//receive until delimiter
	string receive(string);
	//receive given bytes
	string receive(int);
};


#endif //THESIS_CONNECTION_H
