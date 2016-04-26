#ifndef THESIS_PROXYTHREAD_H
#define THESIS_PROXYTHREAD_H

#include <thread>
#include "../TCP/Connection.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Utilities/Lockable.h"
#include "../Network/ObjectRequest.h"
#include "WebsiteDownloader.h"

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

	ConcurrentQueue<ObjectRequest*> queue;
	thread* httpReader;
	thread* httpWriter;
	Connection& connection;
	WebsiteDownloader& downloader;
};


#endif //THESIS_PROXYTHREAD_H
