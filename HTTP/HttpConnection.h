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
	HttpConnection(Connection&);
	~HttpConnection();
	void run();
	void enqueueRequest(Lockable<NetworkRequest>*);
	void join();

private:
	void readerFunction();
	void writerFunction();

	ConcurrentQueue<Lockable<NetworkRequest>*> requestQueue;
	ConcurrentQueue<Lockable<NetworkRequest>*> responseQueue;
	thread* httpReader;
	thread* httpWriter;
	Connection& connection;
};


#endif //THESIS_HTTPCONNECTION_H
