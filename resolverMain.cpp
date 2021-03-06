#include <iostream>
#include <cstring>
#include <thread>
#include <algorithm>
#include <sys/unistd.h>
#include "Utilities/Log.h"
#include "TCP/Connection.h"
#include "TCP/ServerConnection.h"
#include "PSR/PsrMessage.h"
#include "PSR/Resolver.h"
#include "HTTP/HttpRequest.h"
#include "HTTP/HttpResponse.h"

using namespace std;

#define SERVER_PORT 3921

Resolver resolver;
bool active = true;

void pollingThread() {
	while (active) {
		set<string> hosts = resolver.getAllHosts();
		set<string> toRemove;
		for (auto it = hosts.begin(); it != hosts.end(); ++it) {
			Log::f("Polling host " + *it);
			HttpRequest hr;
			hr.method = "GET";
			hr.version = "HTTP/1.0";
			hr.url = "/";
			hr.headers = {{"X-Resolver", "ping"},
						  {"Connection", "close"}};

			Connection c(PsrMessage::addressFromAddress(*it), PsrMessage::portFromAddress(*it));
			c.sendStr(hr.compile());
			if (c.error()) {
				toRemove.insert(*it);
				continue;
			}

			HttpResponse hres(c);
			if (c.error() || hres.responseCode != "204") {
				toRemove.insert(*it);
			}
		}
		for (auto it = toRemove.begin(); it != toRemove.end(); ++it) {
			resolver.remove(*it);
		}
		this_thread::sleep_for(chrono::seconds(5));
	}
}

int main() {
	Log::setLogLevel(LogLevel::TRACE);

	Log::d("Starting server on port 3921");
	ServerConnection sc(SERVER_PORT);

	Log::d("Starting polling thread");
	thread polling(pollingThread);

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
			vector<string> allWebsites = resolver.getAllWebsites();
			sort(hosts.begin(), hosts.end());
			sort(allWebsites.begin(), allWebsites.end());
			vector<string> diff;
			set_difference(allWebsites.begin(), allWebsites.end(), hosts.begin(), hosts.end(),
						   back_insert_iterator<vector<string>>(diff));
			if (diff.size() == 0) {
				for (string host : hosts)
					resolver.add(host, server);
				response.setOk();
			}
			else
				response.setReplicate(diff);
		} else {
			response.setInvalid();
		}

		client->sendStr(response.compile());
		Log::d("Request completed, waiting for new connection.");

		delete client;
	}

	active = false;
	Log::f("Error in receiving connections. Error (code " + to_string(errno) + "):");
	Log::f(strerror(errno));

	return 0;
}