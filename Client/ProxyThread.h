#ifndef THESIS_PROXYTHREAD_H
#define THESIS_PROXYTHREAD_H

#include <thread>
#include "../TCP/Connection.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Utilities/Lockable.h"
#include "../Network/NetworkRequest.h"

using namespace std;

class ProxyThread {
public:
	ProxyThread(Connection&);
	void run();

private:
	void readerFunction();
	void writerFunction();

	ConcurrentQueue<Lockable<NetworkRequest>*> queue;
	thread* httpReader;
	thread* httpWriter;
	Connection& connection;
};


#endif //THESIS_PROXYTHREAD_H
