#ifndef THESIS_PROXYTHREAD_H
#define THESIS_PROXYTHREAD_H

#include <thread>
#include "WebsiteDownloader.h"
#include "../TCP/Connection.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Network/ObjectRequest.h"
#include "../Network/VerifiedObjectRequest.h"

using namespace std;

class ProxyThread {
public:
	ProxyThread(Connection&, WebsiteDownloader&);
	~ProxyThread();
	void run();
	void runAndJoin();
	void join();

private:
	void readerFunction();
	void writerFunction();

	ConcurrentQueue<shared_ptr<VerifiedObjectRequest>> queue;
	thread* httpReader;
	thread* httpWriter;
	Connection& connection;
	WebsiteDownloader& downloader;
};


#endif //THESIS_PROXYTHREAD_H
