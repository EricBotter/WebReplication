#include "ProxyThread.h"

ProxyThread::ProxyThread(Connection& c) : connection(c) { }

void ProxyThread::run() {
	httpReader = new thread(readerFunction, this);
	httpWriter = new thread(writerFunction, this);
}

void ProxyThread::readerFunction() {
	string reqStr = connection.receive("\r\n\r\n");
	while (reqStr != "") {
		HttpRequest hr(reqStr);
		NetworkRequest nr(hr);
		queue.push(new Lockable<NetworkRequest>(nr));
		reqStr = connection.receive("\r\n\r\n");
	}
	queue.push(NULL);
}

void ProxyThread::writerFunction() {
	Lockable<NetworkRequest>* request = queue.pop();
	while (request != NULL) {
		unique_lock<mutex> guard(request->getMutex());
		while (!request->getObject().isCompleted())
			request->getCV().wait(guard);
		// reply to HTTP server
		request = queue.pop();
	}
}

