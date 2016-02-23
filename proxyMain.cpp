#include <sys/errno.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include "TCP/ServerConnection.h"
#include "Utilities/Log.h"
#include "HTTP/HttpRequest.h"
#include "HTTP/HttpResponse.h"

using namespace std;

#define SOCKET_PORT 8000

int main() {
	Log::d("Starting server on port 8000");
	ServerConnection sc(SOCKET_PORT);
	Log::d("Server started. Waiting for connection.");

	Connection* browser;
	while ((browser = sc.takeConn()) != NULL) {
		Log::d("Connection received. Waiting for HTTP request.");
		string received = browser->receive("\r\n\r\n");
		while (received != "") {
			Log::d("Request received");
			HttpRequest request(received);

			if (request.method == "GET") {
				HttpRequest newRequest;
				newRequest.method = "GET";
				size_t index = request.url.substr(7).find('/');
				newRequest.url = index != string::npos ? request.url.substr(index + 7) : "/";
				newRequest.version = "HTTP/1.0";
				newRequest.headers = request.headers;

				Connection newConn(request.headers["Host"], 80);
				newConn.sendStr(newRequest.compile());
				HttpResponse response(newConn);
				browser->sendStr(response.compile());
			} else {
				string error = "HTTP/1.1 501 Not Implemented\r\nConnection: ";
//				error += request.headers["Connection"]; //FIXME: doesn't check for header presence
//				error += "\r\n\r\n";
				error += "close\r\n\r\n";
				browser->sendStr(error);
			}

			received = browser->receive("\r\n\r\n");
		}

		delete browser;
	}

	stringstream ss;
	ss << "Error in receiving connections. Error (code " << errno << "):";
	Log::f(ss.str());
	Log::f(strerror(errno));

	delete browser;

	return 0;
}