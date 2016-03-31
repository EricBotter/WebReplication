#ifndef THESIS_WEBSITEDOWNLOADER_H
#define THESIS_WEBSITEDOWNLOADER_H

#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include "../HTTP/HttpResponse.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Utilities/Lockable.h"
#include "../Network/NetworkRequest.h"

using namespace std;

class WebsiteDownloader {
public:
	WebsiteDownloader();
	~WebsiteDownloader();
	void setActiveCaching(bool);
	void enqueueRequest(Lockable<NetworkRequest>*);

private:
	void threadFunction();
	vector<string> resolve(string);

	map<string, vector<string>> resolutions;
	vector<thread> threads;
	mutex resolutionsMutex;

	ConcurrentQueue<Lockable<NetworkRequest>*> requestQueue;

	bool active = true;
};


#endif //THESIS_WEBSITEDOWNLOADER_H
