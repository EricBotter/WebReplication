#include "ProxyThread.h"

ProxyThread::ProxyThread(Connection& c, WebsiteDownloader& downloader)
		: connection(c), downloader(downloader), httpReader(NULL), httpWriter(NULL) { }

ProxyThread::~ProxyThread() {
	if (httpReader != NULL)
		delete httpReader;
	if (httpWriter != NULL)
		delete httpWriter;
}

void ProxyThread::run() {
	//FIXME: run HttpReader synchronously
	httpReader = new thread(&ProxyThread::readerFunction, this);
	httpWriter = new thread(&ProxyThread::writerFunction, this);
}

void ProxyThread::readerFunction() {
	string reqStr = connection.receive("\r\n\r\n");
	while (reqStr != "") {
		HttpRequest hr(reqStr);
		hr.url = hr.url.substr(hr.url.substr(7).find('/') + 7);
		auto temp = new NetworkRequest(hr);
		queue.push(temp);
		downloader.enqueueRequest(temp);
		reqStr = connection.receive("\r\n\r\n");
	}
	queue.push(NULL);
}

void ProxyThread::writerFunction() {
	NetworkRequest* request;
	while ((request = queue.pop())) {
		request->waitForCompleted();
		connection.sendStr(request->getHttpResponse().compile());
	}
}

void ProxyThread::join() {
	if (httpReader->joinable()) {
		httpReader->join();
	}
	if (httpWriter->joinable()) {
		httpWriter->join();
	}
}


