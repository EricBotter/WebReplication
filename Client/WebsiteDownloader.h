#ifndef THESIS_WEBSITEDOWNLOADER_H
#define THESIS_WEBSITEDOWNLOADER_H

#define NEW_NETWORK

#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include "../HTTP/HttpResponse.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Utilities/Lockable.h"
#include "../Network/NetworkRequest.h"
#include "../HTTP/HttpConnection.h"

using namespace std;

class WebsiteDownloader {
public:
	WebsiteDownloader();
	~WebsiteDownloader();
	void setActiveCaching(bool);
	void enqueueRequest(NetworkRequest*);

private:
	void threadFunction();
	vector<string> resolve(string);

	map<string, vector<string>> resolutions;
	vector<thread> threads;
	mutex resolutionsMutex;

	ConcurrentQueue<NetworkRequest*> requestQueue;

#ifdef NEW_NETWORK
	mutex connectionsMutex;
	map<string, HttpConnection*> connections;
#endif

	bool active = true;
};


#endif //THESIS_WEBSITEDOWNLOADER_H
