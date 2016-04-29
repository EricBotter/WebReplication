#include "HttpConnection.h"
#include "../Utilities/Log.h"

HttpConnection::HttpConnection(string host, uint16_t port)
		: httpReader(NULL), httpWriter(NULL) {
	connection = new Connection(host, port);
}

HttpConnection::~HttpConnection() {
	delete connection;
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

void HttpConnection::enqueueRequest(shared_ptr<ObjectRequest> nr) {
	requestQueue.push(nr);
}

void HttpConnection::writerFunction() {
	shared_ptr<ObjectRequest> request;
	while ((request = requestQueue.pop())) {
		Log::t("HttpConnection (writer) has request for <" + request->getHttpRequest().headers["Host"] +
			   request->getHttpRequest().url + ">");
		responseQueue.push(request);
		connection->sendStr(request->getHttpRequest().compile());
	}
	responseQueue.push(NULL);
	Log::t("HttpConnection (writer) is ending.");
}

void HttpConnection::readerFunction() {
	shared_ptr<ObjectRequest> request;
	while ((request = responseQueue.pop())) {
		Log::t("HttpConnection (reader) has request for <" + request->getHttpRequest().headers["Host"] +
			   request->getHttpRequest().url + ">");
		HttpResponse hr(*connection);
		request->setHttpResponse(hr);
		request->setCompleted();

		//TODO: add responseCode checking
	}
	Log::t("HttpConnection (reader) is ending.");
}

void HttpConnection::join() {
	requestQueue.push(NULL);
	if (httpReader->joinable()) {
		httpReader->join();
	}
	if (httpWriter->joinable()) {
		httpWriter->join();
	}
}
