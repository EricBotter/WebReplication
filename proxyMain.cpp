#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include "Utilities/Log.h"
#include "TCP/ServerConnection.h"
#include "Client/ProxyThread.h"
#include "PSR/PsrMessage.h"

using namespace std;

#define SERVER_PORT 8000

WebsiteDownloader downloader;

void connectionThread(Connection* browser) {
	ProxyThread pt(*browser, downloader);
	Log::d("! Connection received. Waiting for HTTP request.");
	pt.run();
	Log::e("@ Threads run");
	pt.join();
	Log::e("X Threads ended");
	delete browser;
	Log::e("# Browser deleted");
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