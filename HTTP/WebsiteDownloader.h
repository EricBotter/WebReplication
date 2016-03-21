#ifndef THESIS_WEBSITEDOWNLOADER_H
#define THESIS_WEBSITEDOWNLOADER_H

#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include "HttpResponse.h"

using namespace std;

class WebsiteDownloader {
public:
	WebsiteDownloader();
	~WebsiteDownloader();
	void setActiveCaching(bool);
	HttpResponse getWebpage(string, string);

private:
	void threadFunction();
	vector<string> resolve(string);

	map<string, vector<string>> resolutions;
	vector<thread> threads;
	queue<pair<string, string>> requestQueue;
	map<pair<string, string>, string> webpageCache;

	mutex requestQueueMutex;
	condition_variable requestQueueCV;
	mutex webpageCacheMutex;
	condition_variable webpageCacheCV;
	mutex resolutionsMutex;
	bool active = true;
};


#endif //THESIS_WEBSITEDOWNLOADER_H
