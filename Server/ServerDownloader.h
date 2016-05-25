#ifndef THESIS_SERVERDOWNLOADER_H
#define THESIS_SERVERDOWNLOADER_H


#include "../Utilities/ConcurrentQueue.h"
#include "../Network/VerifiedObjectRequest.h"
#include "../Client/WebsiteDownloader.h"
#include <thread>

class ServerDownloader {
private:
	ConcurrentQueue<shared_ptr<VerifiedObjectRequest>> requestQueue;
	WebsiteDownloader wd;
	thread* downloaderThread;
public:
	ServerDownloader();
	~ServerDownloader();

	bool enqueueWebsite(const string&);
	void threadFunction();
};


#endif //THESIS_SERVERDOWNLOADER_H
