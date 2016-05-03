#include "HttpClientConnection.h"
#include "../Utilities/Log.h"

HttpClientConnection::HttpClientConnection(string host, uint16_t port)
		: httpReader(NULL), httpWriter(NULL), active(true) {
	connection = new Connection(host, port);
	connection->setTimeout(700);
}

HttpClientConnection::~HttpClientConnection() {
	delete connection;
	if (httpWriter != NULL)
		delete httpWriter;
	if (httpReader != NULL) {
		delete httpReader;
	}
}

void HttpClientConnection::run() {
	httpWriter = new thread(&HttpClientConnection::writerFunction, this);
	httpReader = new thread(&HttpClientConnection::readerFunction, this);
}

void HttpClientConnection::enqueueRequest(shared_ptr<ObjectRequest> nr) {
	requestQueue.push(nr);
}

void HttpClientConnection::writerFunction() {
	shared_ptr<ObjectRequest> request;
	while ((request = requestQueue.pop())) {
		Log::t("HttpClientConnection (writer) has request for <" + request->getHttpRequest().headers["Host"] +
			   request->getHttpRequest().url + ">");
		if (request->getHttpRequest().method == "GET") {
			if (connection->sendStr(request->getHttpRequest().compile()) > 0) {
				responseQueue.push(request);
			} else {
				request->setFailed();
				makeInactive();
			}
		}
		else if (request->getHttpRequest().method == "CONNECT") {
			//TODO: support HTTPS
		}
	}
	makeInactive();
	Log::t("HttpClientConnection (writer) is ending.");
}

void HttpClientConnection::readerFunction() {
	shared_ptr<ObjectRequest> request;
	while ((request = responseQueue.pop())) {
		Log::t("HttpClientConnection (reader) has request for <" + request->getHttpRequest().headers["Host"] +
			   request->getHttpRequest().url + ">");
		if (request->getHttpRequest().method == "GET") {
			//TODO: add responseCode checking
			HttpResponse hr(*connection);
			if (connection->error()) {
				request->setFailed();
				makeInactive();
			} else {
				request->setHttpResponse(hr);
				request->setCompleted();
			}
		} else {
			//TODO: make this static
			HttpResponse notImplemented;
			notImplemented.version = "HTTP/1.0";
			notImplemented.responseCode = "501";
			notImplemented.responseText = "Not Implemented";
			notImplemented.headers = {
					{"Content-Length", "0"},
					{"Connection",     "keep-alive"}
			};
			notImplemented.contentLength = 0;
			request->setHttpResponse(notImplemented);
			request->setCompleted();
		}
	}
	Log::t("HttpClientConnection (reader) is ending.");
}

void HttpClientConnection::join() {
	makeInactive();
	if (httpReader->joinable()) {
		httpReader->join();
	}
	if (httpWriter->joinable()) {
		httpWriter->join();
	}
}

bool HttpClientConnection::isActive() {
	return active;
}

void HttpClientConnection::makeInactive() {
	if (!active) {
		active = false;
		requestQueue.push(NULL);
	}
}
