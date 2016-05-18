#ifndef THESIS_SERVERCONNECTION_H
#define THESIS_SERVERCONNECTION_H


#include "Connection.h"

class ServerConnection {
private:
	int sockfd;
	int lastError = 0;
public:
	ServerConnection(uint16_t);
	~ServerConnection();

	Connection* takeConn();
	int error();
};


#endif //THESIS_SERVERCONNECTION_H
