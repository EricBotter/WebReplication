#ifndef THESIS_HTTPCONNECTION_H
#define THESIS_HTTPCONNECTION_H

#include <thread>
#include "../TCP/Connection.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Utilities/Lockable.h"
#include "../Network/NetworkRequest.h"

using namespace std;

class HttpConnection {
public:
	HttpConnection(string, uint16_t);
	~HttpConnection();
	void run();
	void enqueueRequest(NetworkRequest*);
	void join();

private:
	void readerFunction();
	void writerFunction();

	ConcurrentQueue<NetworkRequest*> requestQueue;
	ConcurrentQueue<NetworkRequest*> responseQueue;
	thread* httpReader;
	thread* httpWriter;
	Connection* connection;
};


#endif //THESIS_HTTPCONNECTION_H
