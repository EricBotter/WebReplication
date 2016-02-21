#ifndef THESIS_SERVERCONNECTION_H
#define THESIS_SERVERCONNECTION_H


#include "Connection.h"

class ServerConnection {
private:
	int sockfd;
public:
	ServerConnection(uint16_t);

	//error checking function
	Connection* takeConn();
};


#endif //THESIS_SERVERCONNECTION_H
