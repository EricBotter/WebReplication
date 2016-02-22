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
	Connection(const string&, uint16_t);
	~Connection();
	int sendStr(const string&) const;
	//receive until delimiter
	string receive(const string&);
	//receive given bytes
	string receive(size_t);
};


#endif //THESIS_CONNECTION_H
