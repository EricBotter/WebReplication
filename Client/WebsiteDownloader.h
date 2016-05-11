#ifndef THESIS_WEBSITEDOWNLOADER_H
#define THESIS_WEBSITEDOWNLOADER_H

#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include "../HTTP/HttpResponse.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Network/ObjectRequest.h"
#include "../HTTP/HttpClientConnection.h"
#include "../Network/VerifiedObjectRequest.h"

using namespace std;

class WebsiteDownloader {
public:
	WebsiteDownloader();
	~WebsiteDownloader();
	void enqueueRequest(shared_ptr<VerifiedObjectRequest>);
	void setResolverAddress(const string&);

private:
	void threadFunction();
	vector<string> resolve(const string&);
	shared_ptr<HttpClientConnection> serverFromWebsite(const string&);

	map<string, vector<string>> resolutionCache;
	vector<thread> threads;
	mutex resolutionsMutex;

	ConcurrentQueue<shared_ptr<VerifiedObjectRequest>> requestQueue;
	mutex connectionsMutex;
	map<string, shared_ptr<HttpClientConnection>> connections;

	bool active = true;
	string resolverAddress;
};


#endif //THESIS_WEBSITEDOWNLOADER_H
