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
	httpReader = new thread(&ProxyThread::readerFunction, this);
	httpWriter = new thread(&ProxyThread::writerFunction, this);
}

void ProxyThread::readerFunction() {
	string reqStr = connection.receive("\r\n\r\n");
	while (reqStr != "") {
		HttpRequest hr(reqStr);
		hr.url = hr.url.substr(hr.url.substr(7).find('/') + 7);
		NetworkRequest nr(hr);
		auto temp = new Lockable<NetworkRequest>(nr);
		queue.push(temp);
		downloader.enqueueRequest(temp);
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
		connection.sendStr(request->getObject().getHttpResponse().compile());
		request = queue.pop();
	}
}

void ProxyThread::join() {
	if (httpReader->joinable()) {
		httpWriter->join();
	}
	if (httpWriter->joinable()) {
		httpWriter->join();
	}
}


