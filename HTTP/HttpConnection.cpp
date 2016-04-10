#include "HttpConnection.h"

HttpConnection::HttpConnection(Connection& c)
		: connection(c), httpReader(NULL), httpWriter(NULL) { }

HttpConnection::~HttpConnection() {
	if (httpWriter != NULL)
		delete httpWriter;
	if (httpReader != NULL) {
		delete httpReader;
	}
}

void HttpConnection::run() {
	httpWriter = new thread(&HttpConnection::writerFunction, this);
	httpReader = new thread(&HttpConnection::readerFunction, this);
}

void HttpConnection::enqueueRequest(Lockable<NetworkRequest>* nr) {
	requestQueue.push(nr);
}

void HttpConnection::writerFunction() {
	Lockable<NetworkRequest>* request = requestQueue.pop();
	responseQueue.push(request);
	while (request != NULL) {
//		unique_lock<mutex> guard(request->getMutex());
//		while (!request->getObject().isCompleted())
//			request->getCV().wait(guard);
		connection.sendStr(request->getObject().getHttpRequest().compile());

		request = requestQueue.pop();
	}
	responseQueue.push(NULL);
}

void HttpConnection::readerFunction() {
	Lockable<NetworkRequest>* request = responseQueue.pop();
	while (request != NULL) {
		HttpResponse hr(connection);
		unique_lock<mutex> guard(request->getMutex());

		if (hr.responseCode == "200" || //assume request succeeded and all data is here
				hr.responseCode[0] == '4') {  //assume request failed
			request->getObject().setHttpResponse(hr);
			request->getObject().setCompleted(true);
			request->getCV().notify_one();
//		} else if (hr.responseCode == "206") { //request is incomplete
//			//TODO: handle chunked content by auto-queuing next request
		}

		request = responseQueue.pop();
	}
}

void HttpConnection::join() {
	if (httpReader->joinable()) {
		httpReader->join();
	}
	if (httpWriter->joinable()) {
		httpWriter->join();
	}
}




