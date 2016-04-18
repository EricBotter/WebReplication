#include "ProxyThread.h"
#include "../Utilities/Log.h"

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

void ProxyThread::runAndJoin() {
	httpWriter = new thread(&ProxyThread::writerFunction, this);
	readerFunction();
	httpWriter->join();
}

void ProxyThread::readerFunction() {
	string reqStr = connection.receive("\r\n\r\n");
	while (reqStr != "") {
		HttpRequest hr(reqStr);
		hr.url = hr.url.substr(hr.url.substr(7).find('/') + 7);
		auto temp = new NetworkRequest(hr);
		queue.push(temp);
		downloader.enqueueRequest(temp);
		Log::t("Enqueued request for url <" + hr.url + "> of site <" + hr.headers["Host"] + '>');
		reqStr = connection.receive("\r\n\r\n");
	}
	queue.push(NULL);
}

void ProxyThread::writerFunction() {
	NetworkRequest* request;
	while ((request = queue.pop())) {
		Log::t("Waiting for request of url <" + request->getHttpRequest().url + "> of site <" + request->getHttpRequest().headers["Host"] + '>');
		request->waitForCompleted();
		connection.sendStr(request->getHttpResponse().compile());
		Log::t("Completed request of url <" + request->getHttpRequest().url + "> of site <" + request->getHttpRequest().headers["Host"] + '>');
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
