#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <sys/errno.h>
#include "Utilities/Log.h"
#include "TCP/ServerConnection.h"
#include "HTTP/HttpRequest.h"
#include "HTTP/HttpResponse.h"
#include "HTTP/FileServer.h"
#include "PSR/PsrMessage.h"

using namespace std;

#define SERVER_PORT 4100

string notfound = "<html><head><title>Not found</title></head><body><h1>Not found</h1>The requested object could not be found on this server</body></html>";
FileServer fs;

void connectionThread(Connection* client) {
	Log::d("! Connection received. Waiting for HTTP request.");
	string received = client->receive("\r\n\r\n");

	while (received != "") {
		HttpRequest request(received);
		Log::d(string("_ Request received for URL ") + request.url);

		if (request.method == "GET") {
			HttpResponse response;

			response.version = "HTTP/1.0";

			string host = request.headers["Host"];
			string content = fs.getFile(host, request.url);
			if (content == "" && *request.url.rbegin() == '/') {
				content = fs.getFile(host, request.url + "index");
				if (content == "") {
					content = fs.getFile(host, request.url + "index.html");
				}
			}
			if (content == "") {
				response.responseCode = "404";
				response.responseText = "Not Found";

				stringstream ss;
				ss << notfound.length();

				response.headers.insert({"Connection", "keep-alive"});
				response.headers.insert({"Content-Type", "text/html"});
				response.headers.insert({"Content-Length", ss.str()});

				response.contentLength = notfound.length();
				response.content = new char[notfound.length()];
				memcpy(response.content, notfound.c_str(), notfound.length());
			} else {
				response.responseCode = "200";
				response.responseText = "OK";

				stringstream ss;
				ss << content.length();

				response.headers.insert({"Content-Type", FileServer::mimeFromFilename(request.url)});
				response.headers.insert({"Content-Length", ss.str()});
				response.headers.insert({"Connection", "keep-alive"});

				response.contentLength = content.length();
				response.content = new char[content.length()];
				memcpy(response.content, content.c_str(), content.length());
			}

			client->sendStr(response.compile());
			if (response.headers["Connection"] == "close")
				break;
		} else {
			client->sendStr("HTTP/1.1 501 Not Implemented\r\nConnection: close\r\n\r\n");
			break;
		}
		Log::d("< Request completed");

		received = client->receive("\r\n\r\n");
	}

	Log::d("x Closing connection to client");
	delete client;
}

int main() {
	Log::d("Loading websites...");
	fs.init();
	Log::d("Announcing sites to resolver");
	Connection* c = new Connection("127.0.0.1", 3921, "127.0.0.1", SERVER_PORT);
	PsrMessage pm;
	pm.setSites(fs.getSiteList());
	c->sendStr(pm.compile());
	pm = PsrMessage(*c);
	if (pm.value != "OK") {
		Log::e("ERROR - Resolver didn't receive site list");
	}

	delete c;

	Log::d("Starting server on custom port");
	ServerConnection sc(SERVER_PORT);
	Log::d("Server started. Waiting for connection.");

	Connection* client;
	while ((client = sc.takeConn()) != NULL) {
		thread(connectionThread, client).detach();
	}

	stringstream ss;
	ss << "Error in receiving connections. Error (code " << errno << "):";
	Log::f(ss.str());
	Log::f(strerror(errno));

	return 0;
}