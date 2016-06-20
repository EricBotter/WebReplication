#ifndef THESIS_SERVERDOWNLOADER_H
#define THESIS_SERVERDOWNLOADER_H


#include "../Utilities/ConcurrentQueue.h"
#include "../Network/VerifiedObjectRequest.h"
#include "../Client/WebsiteDownloader.h"
#include <thread>

class ServerDownloader {
private:
	ConcurrentQueue<shared_ptr<VerifiedObjectRequest>> requestQueue;
	ConcurrentQueue<string> websiteQueue;
	thread* downloaderThread;
public:
	ServerDownloader(const string&);
	~ServerDownloader();

	bool enqueueWebsite(const string&);
	void threadFunction();
};


#endif //THESIS_SERVERDOWNLOADER_H
