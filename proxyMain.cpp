#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <sys/errno.h>
#include "Utilities/Log.h"
#include "TCP/ServerConnection.h"
#include "HTTP/HttpRequest.h"
#include "HTTP/HttpResponse.h"
#include "PSR/PsrMessage.h"

using namespace std;

#define SERVER_PORT 8000

void connectionThread(Connection* browser) {
	Log::d("! Connection received. Waiting for HTTP request.");
	string received = browser->receive("\r\n\r\n");
	while (received != "") {
		HttpRequest request(received);
		Log::d(string("_ Request received for URL ")+request.url);

		if (request.method == "GET") {
			HttpRequest newRequest;
			newRequest.method = "GET";

			if (request.url.find(".peer/") != string::npos) {
				PsrMessage psrRequest;
				psrRequest.key = "Host";
				psrRequest.value = request.url.substr(7, request.url.substr(7).find("/"));

				Connection psrConn("127.0.0.1", 3921);
				psrConn.sendStr(psrRequest.compile());
				PsrMessage psrResponse(psrConn);
				vector<string> hosts = psrResponse.getHosts();

				newRequest.version = "HTTP/1.0";
				newRequest.url = request.url.substr(7 + psrRequest.value.length());
				newRequest.headers = request.headers;
				newRequest.headers["Connection"] = "close";

				Connection newConn(PsrMessage::addressFromAddress(hosts[0]), PsrMessage::portFromAddress(hosts[0]));
				newConn.sendStr(newRequest.compile());
				HttpResponse response(newConn);
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
			browser->sendStr("HTTP/1.1 501 Not Implemented\r\nConnection: close\r\nProxy-Connection: close\r\n\r\n");
			break;
		}
		Log::d("< Request completed");

		received = browser->receive("\r\n\r\n");
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