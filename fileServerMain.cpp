#include <iostream>
#include <cstring>
#include <sstream>
#include <sys/errno.h>
#include "Utilities/Log.h"
#include "TCP/ServerConnection.h"
#include "HTTP/HttpRequest.h"
#include "HTTP/HttpResponse.h"
#include "HTTP/FileServer.h"
#include "PSR/PsrMessage.h"

using namespace std;

#define SERVER_PORT 4100

int main() {
	FileServer fs;
	Log::d("Announcing sites to resolver");
	Connection* c = new Connection("127.0.0.1", 3921, "127.0.0.1", SERVER_PORT);
	PsrMessage pm;
	pm.setSites(fs.getSiteList());
	c->sendStr(pm.compile());
	pm = PsrMessage(*c);
	Log::d(pm.key);
	Log::d(pm.value);
	if (pm.value != "OK") {
		Log::e("ERROR - Resolver didn't receive site list");
	}

	delete c;

	Log::d("Starting server on custom port");
	ServerConnection sc(SERVER_PORT);
	Log::d("Server started. Waiting for connection.");

	Connection* client;
	while ((client = sc.takeConn()) != NULL) {
		Log::d("! Connection received. Waiting for HTTP request.");
		string received = client->receive("\r\n\r\n");

		if (received != "") {
			HttpRequest request(received);
			Log::d(string("_ Request received for URL ") + request.url);

			if (request.method == "GET") {
				HttpResponse response;

				string host = request.headers["Host"];
				string content = fs.getFile(host, request.url);

				response.version = request.version;
				response.responseCode = "200";
				response.responseText = "OK";

				stringstream ss;
				ss << content.length();

				response.headers.insert({"Content-Type", "text/html"});
				response.headers.insert({"Content-Length", ss.str()});
				response.headers.insert({"Connection", "close"});

				response.contentLength = content.length();
				response.content = new char[content.length()];
				memcpy(response.content, content.c_str(), content.length());

				client->sendStr(response.compile());
			} else {
				client->sendStr("HTTP/1.1 501 Not Implemented\r\nConnection: close\r\n\r\n");
				break;
			}
			Log::d("< Request completed");
		}

		delete client;
		Log::d(". Waiting for new connection...");
	}

	stringstream ss;
	ss << "Error in receiving connections. Error (code " << errno << "):";
	Log::f(ss.str());
	Log::f(strerror(errno));

	return 0;
}