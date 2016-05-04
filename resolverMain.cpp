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
		Log::d("Connection received. Waiting for PSR request.");
		PsrMessage request(*client);

		Log::d(string("Request received: " + request.message));

		PsrMessage response;
		if (request.message == "RESOLVE" && request.values.begin()->first == "Host") {
			vector<string> addresses = resolver.resolve(request.values.begin()->second);
			response.setAddresses(addresses);
		} else if (request.message == "ANNOUNCE") {
			string server;
			vector<string> hosts = request.getAnnounced(server);
			for(string host : hosts)
				resolver.add(host, server);
			response.setOk();
		} else {
			response.setInvalid();
		}

		client->sendStr(response.compile());
		Log::d("Request completed, waiting for new connection.");

		delete client;
	}

	Log::f("Error in receiving connections. Error (code " + to_string(errno) + "):");
	Log::f(strerror(errno));

	return 0;
}