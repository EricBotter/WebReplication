#ifndef THESIS_HTTPCLIENTCONNECTION_H
#define THESIS_HTTPCLIENTCONNECTION_H

#include <thread>
#include "../TCP/Connection.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Network/ObjectRequest.h"

using namespace std;

class HttpClientConnection {
public:
	HttpClientConnection(string, uint16_t);
	~HttpClientConnection();
	void run();
	void enqueueRequest(shared_ptr<ObjectRequest>);
	void join();
	bool isActive();
	size_t queueLength();

private:
	void readerFunction();
	void writerFunction();
	void makeInactive();

	ConcurrentQueue<shared_ptr<ObjectRequest>> requestQueue;
	ConcurrentQueue<shared_ptr<ObjectRequest>> responseQueue;
	thread* httpReader;
	thread* httpWriter;
	Connection* connection;
	bool active;
};


#endif //THESIS_HTTPCLIENTCONNECTION_H
