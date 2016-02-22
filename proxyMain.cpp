#include <sys/errno.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include "TCP/ServerConnection.h"
#include "Utilities/Log.h"

using namespace std;

#define SOCKET_PORT 8000

int main() {
	Log::d("Starting server on port 8000");
	ServerConnection sc(SOCKET_PORT);
	Log::d("Server started. Waiting for connection.");

	Connection* client = sc.takeConn();
	if (client == NULL) {
		stringstream ss;
		ss << "Unable to receive connections. Error (code " << errno << "):";
		Log::f(ss.str());
		Log::f(strerror(errno));
		return -1;
	}

	Log::d("Connection received. Sending hello");
	client->sendStr("YOOOO M8\n");
	string line = client->receive("\n");
	Log::d("Line received:");
	Log::d(line);

	client->sendStr("Ok m8 now send me 10 bytes\n");
	line = client->receive(10);
	Log::d("Bytes received:");
	Log::d(line);

	delete client;

	return 0;
}