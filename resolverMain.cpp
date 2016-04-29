#include <iostream>
#include <cstring>
#include <sstream>
#include <sys/errno.h>
#include <sys/unistd.h>
#include "Utilities/Log.h"
#include "TCP/Connection.h"
#include "TCP/ServerConnection.h"
#include "PSR/PsrMessage.h"
#include "PSR/Resolver.h"

using namespace std;

#define SERVER_PORT 3921

int main() {
	Log::setLogLevel(LogLevel::TRACE);

	Log::d("Starting server on port 3921");
	Resolver resolver;
	ServerConnection sc(SERVER_PORT);
	Log::d("Server started. Waiting for connection.");

	Connection* client;
	while ((client = sc.takeConn()) != NULL) {
		Log::d("! Connection received. Waiting for PSR request.");
		PsrMessage request(*client);

		Log::d(string("_ Request received for URL ")+request.value);

		PsrMessage response;
		if (request.key == "Host") {
			vector<string> addresses = resolver.resolve(request.value);
			response.setAddresses(addresses);
		} else if (request.key == "Available") {
			vector<string> hosts = request.getHosts();
			for(string host : hosts)
				resolver.add(host, client->getRemoteAddress());
			response.setMessage("OK");
		} else {
			response.setMessage("INVALID");
		}

		client->sendStr(response.compile());
		Log::d("< Request completed");

		delete client;
		Log::d(". Waiting for new connection...");
	}

	stringstream ss;
	ss << "Error in receiving connections. Error (code " << errno << "):";
	Log::f(ss.str());
	Log::f(strerror(errno));

	return 0;
}