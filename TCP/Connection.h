#ifndef THESIS_CONNECTION_H
#define THESIS_CONNECTION_H

#include <string>

using namespace std;

#define RECV_BUFFER_SIZE 2048

class Connection {
private:
	int sockfd;
	size_t recvBufPos;
	char recvBuffer[RECV_BUFFER_SIZE];
public:
	//To be used by servers
	Connection(int);
	//To be used by clients
	Connection(string, int);
	~Connection();
	int sendStr(string) const;
	//receive until delimiter
	string receive(string);
	//receive given bytes
	string receive(int);
};


#endif //THESIS_CONNECTION_H
