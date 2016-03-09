#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <cstdlib>
#include <sys/errno.h>
#include "Utilities/Log.h"
#include "TCP/ServerConnection.h"
#include "HTTP/HttpRequest.h"
#include "HTTP/HttpResponse.h"
#include "PSR/PsrMessage.h"

using namespace std;

#define SERVER_PORT 8000

map<string, Connection*> connections;
map<string, vector<string>> resolutions;

void connectionThread(Connection* browser) {
	Log::d("! Connection received. Waiting for HTTP request.");
	string received = browser->receive("\r\n\r\n");
	Connection* currentConnection = NULL;
	string currentHost;
	while (received != "") {
		HttpRequest request(received);
		Log::d(string("_ Request received for URL ") + request.url);

		if (request.method == "GET") {
			HttpRequest newRequest;
			newRequest.method = "GET";

			if (request.url.find(".peer/") != string::npos) {
				vector<string> hosts;
				string hostName = request.url.substr(7, request.url.substr(7).find("/"));

				if (resolutions.find(hostName) != resolutions.end()) {
					hosts = resolutions[hostName];
				} else {
					PsrMessage psrRequest;
					psrRequest.key = "Host";
					psrRequest.value = hostName;

					//FIXME: hardcoded resolution IP and PORT
					Connection psrConn("127.0.0.1", 3921);
					psrConn.sendStr(psrRequest.compile());
					PsrMessage psrResponse(psrConn);
					hosts = psrResponse.getHosts();
					resolutions.insert({hostName, hosts});
				}

				newRequest.version = "HTTP/1.0";
				newRequest.url = request.url.substr(7 + hostName.length());
				newRequest.headers = request.headers;

				if (currentConnection == NULL) {
					for (int i = 0; i < hosts.size(); ++i) {
						if (connections.find(hosts[i]) == connections.end()) {
							currentConnection = new Connection(PsrMessage::addressFromAddress(hosts[i]),
															   PsrMessage::portFromAddress(hosts[i]));
							currentHost = hosts[i];
							break;
						}
					}
					if (currentConnection == NULL) {
						unsigned long temp = rand() % hosts.size();
						Log::d(" - Random host");
						currentConnection = new Connection(PsrMessage::addressFromAddress(hosts[temp]),
														   PsrMessage::portFromAddress(hosts[temp]));
						currentHost = hosts[temp];
					}
					connections[currentHost] = currentConnection;
					Log::d(string("Connecting to host ") + currentHost);
				} else {
					Log::d(string("Using existent connection to host ") + currentHost);
				}

				currentConnection->sendStr(newRequest.compile());
				HttpResponse response(*currentConnection);
				browser->sendStr(response.compile());

			} else {

				size_t index = request.url.substr(7).find('/');
				newRequest.url = index != string::npos ? request.url.substr(index + 7) : "/";
				newRequest.version = "HTTP/1.0";
				newRequest.headers = request.headers;
				newRequest.headers["Connection"] = "close";

				Connection newConn(request.headers["Host"], 80);
				newConn.sendStr(newRequest.compile());
				HttpResponse response(newConn);
				browser->sendStr(response.compile());
			}
		} else {
			browser->sendStr("HTTP/1.1 501 Not Implemented\r\nConnection: close\r\n\r\n");
			break;
		}
		Log::d("< Request completed");
		if (request.headers["Connection"] == "close") {
			break;
		}

		received = browser->receive("\r\n\r\n");
	}

	if (currentHost != "") {
		//FIXME: Other connections to this host from other threads are not considered
		connections.erase(currentHost);
		delete currentConnection;
		Log::d("Dropping connection to " + currentHost);
	}
	delete browser;
}

int main() {
	Log::d("Starting server on port 8000");
	ServerConnection sc(SERVER_PORT);
	Log::d("Server started. Waiting for connection.");

	Connection* browser;
	while ((browser = sc.takeConn()) != NULL) {
		thread(connectionThread, browser).detach();
	}

	stringstream ss;
	ss << "Error in receiving connections. Error (code " << errno << "):";
	Log::f(ss.str());
	Log::f(strerror(errno));

	return 0;
}