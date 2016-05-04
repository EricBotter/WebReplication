#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <sys/errno.h>
#include <unistd.h>
#include <regex>
#include "Utilities/Log.h"
#include "TCP/ServerConnection.h"
#include "HTTP/HttpRequest.h"
#include "HTTP/HttpResponse.h"
#include "HTTP/FileServer.h"
#include "PSR/PsrMessage.h"
#include "Utilities/IniReader.h"

using namespace std;

string confFilePath = "/var/webr/server.conf";
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

			string content;

			if (request.url.find("?sig", request.url.length() - 4) != string::npos) {
				string originalUrl = request.url.substr(0, request.url.length() - 4);
				content = fs.getSignature(host, originalUrl + ".sig");
				if (content == "" && request.url == "/?sig") {
					content = fs.getSignature(host, originalUrl + "index.sig");
					if (content == "") {
						content = fs.getSignature(host, originalUrl + "index.html.sig");
					}
				}
			}
			else {
				content = fs.getFile(host, request.url);
				if (content == "" && *request.url.rbegin() == '/') {
					content = fs.getFile(host, request.url + "index");
					if (content == "") {
						content = fs.getFile(host, request.url + "index.html");
					}
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

int main(int argc, char* argv[]) {
	string serverAddress = "127.0.0.1:4011"; //default address and port
	string resolverAddress = "127.0.0.1:3921"; //default resolver

	Log::d("Reading configuration file...");
	IniReader ir;
	ir.readFromFile(confFilePath);
	if (ir.properties.find("address") != ir.properties.end()) {
		if (regex_match(ir.properties["address"], regex("[a-z.0-9]+:[0-9]{1,5}")))
			serverAddress = ir.properties["address"];
	}
	if (ir.properties.find("resolver") != ir.properties.end()) {
		if (regex_match(ir.properties["resolver"], regex("[a-z.0-9]+:[0-9]{1,5}")))
			resolverAddress = ir.properties["resolver"];
	}
	if (ir.properties.find("loglevel") != ir.properties.end()) {
		string level = ir.properties["loglevel"];
		if (level == "trace") {
			Log::setLogLevel(LogLevel::TRACE);
		} else if (level == "debug") {
			Log::setLogLevel(LogLevel::DEBUG);
		} else if (level == "warn") {
			Log::setLogLevel(LogLevel::WARN);
		} else if (level == "error") {
			Log::setLogLevel(LogLevel::ERROR);
		}
	}

	uint16_t serverport = PsrMessage::portFromAddress(serverAddress);

	Log::d("Announcing sites to resolver...");
	Connection* c = new Connection(
			PsrMessage::addressFromAddress(resolverAddress),
			PsrMessage::portFromAddress(resolverAddress),
			"127.0.0.1", //must be a local address, do not change
			serverport
	);
	PsrMessage pm;
	pm.setSites(fs.getSiteList(), serverAddress);
	c->sendStr(pm.compile());
	pm = PsrMessage(*c);
	if (pm.message != "OK") {
		Log::f("FATAL ERROR - Resolver didn't receive site list");
		return EXIT_FAILURE;
	}

	delete c;

	Log::d("Starting server on port " + to_string(serverport));
	ServerConnection sc(serverport);
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