#ifndef THESIS_HTTPCONNECTION_H
#define THESIS_HTTPCONNECTION_H

#include <thread>
#include "../TCP/Connection.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Network/ObjectRequest.h"

using namespace std;

class HttpConnection {
public:
	HttpConnection(string, uint16_t);
	~HttpConnection();
	void run();
	void enqueueRequest(shared_ptr<ObjectRequest>);
	void join();

private:
	void readerFunction();
	void writerFunction();

	ConcurrentQueue<shared_ptr<ObjectRequest>> requestQueue;
	ConcurrentQueue<shared_ptr<ObjectRequest>> responseQueue;
	thread* httpReader;
	thread* httpWriter;
	Connection* connection;
};


#endif //THESIS_HTTPCONNECTION_H
