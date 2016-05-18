#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <regex>
#include "Utilities/Log.h"
#include "Utilities/IniReader.h"
#include "TCP/ServerConnection.h"
#include "Client/ProxyThread.h"

using namespace std;

#define SERVER_PORT 8000

const string confFilePath = "/var/webr/proxy.conf";

WebsiteDownloader downloader;

void connectionThread(Connection* browser) {
	ProxyThread pt(*browser, downloader);
	Log::d("Connection received. Waiting for HTTP request.");
	pt.runAndJoin();
	delete browser;
	Log::t("ProxyThread ended. Connection with browser closed.");
}

int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp(argv[1], "--program-log")) {
		Log::enableLogToProgram();
	}

	Log::setLogLevel(LogLevel::TRACE);

	Log::d("Reading configuration file...");
	IniReader ir;
	ir.readFromFile(confFilePath);
	if (ir.properties.find("verification") != ir.properties.end()) {
		if (ir.properties["verification"] == "disabled")
			; //disable verification
	}
	if (ir.properties.find("resolver") != ir.properties.end()) {
		if (regex_match(ir.properties["resolver"], regex("[a-z.0-9]+:[0-9]{1,5}")))
			downloader.setResolverAddress(ir.properties["resolver"]);
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

	Log::d("Starting server on port " + to_string(SERVER_PORT));
	LOG_P("START " + to_string(SERVER_PORT));
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
	LOG_P("STOP");

	return 0;
}