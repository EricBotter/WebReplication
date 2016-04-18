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
	int errorCode;
public:
	//To be used by servers
	Connection(int);
	//To be used by clients
	Connection(const string&, uint16_t);
	Connection(const string&, uint16_t, const string&, uint16_t); //Deprecated
	~Connection();
	int sendStr(const string&) const;
	//receive until delimiter
	string receive(const string&);
	//receive given bytes
	string receive(size_t);
	//get host's IP -- deprecated
	string getRemoteAddress();

	// Is non-zero if connection fails (equivalent of errno)
	int error();
};


#endif //THESIS_CONNECTION_H
