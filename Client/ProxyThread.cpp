#include <cstring>
#include "ProxyThread.h"
#include "../Utilities/Log.h"

string notVerifiedEntity = string("<html><head><title>Website not verified</title></head><body>") +
						   "<h2>Unable to verify the content of the requested resource</h2>" +
						   "</body></html>";
string notVerifiableEntity = string("<html><head><title>Website not verifiable</title></head><body>") +
							 "<h2>Unable to verify the content of the requested resource</h2>" +
							 "The proxy does not have the public key needed to verify the content of this website." +
							 "</body></html>";

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
		shared_ptr<ObjectRequest> oReq = make_shared<ObjectRequest>(hr);
		shared_ptr<VerifiedObjectRequest> temp = make_shared<VerifiedObjectRequest>(oReq);
		queue.push(temp);
		downloader.enqueueRequest(temp);
		Log::t("Enqueued request for url <" + hr.url + "> of site <" + hr.headers["Host"] + '>');
		reqStr = connection.receive("\r\n\r\n");
	}
	queue.push(NULL);
}

void ProxyThread::writerFunction() {
	shared_ptr<VerifiedObjectRequest> request;
	while ((request = queue.pop())) {
		Log::t("Waiting for request of url <" + request->getObjectUrl() + "> of site <" + request->getWebsite() + '>');
		request->waitForVerification();
		if (request->canBeVerified()) {
			if (request->isVerified()) {
				connection.sendStr(request->getObject()->getHttpResponse().compile());
			} else {
				//TODO: make this "static"
				HttpResponse notVerified;
				notVerified.version = "HTTP/1.0";
				notVerified.responseCode = "500";
				notVerified.responseText = "Internal Server Error";
				notVerified.headers = {
						{"Content-Type",   "text/html"},
						{"Content-Length", to_string(notVerifiedEntity.length())},
						{"Connection",     "keep-alive"}
				};
				notVerified.contentLength = notVerifiedEntity.length();
				notVerified.content = new char[notVerifiedEntity.length()];
				memcpy(notVerified.content, notVerifiedEntity.c_str(), notVerified.contentLength);
				connection.sendStr(notVerified.compile());
			}
		} else {
			if (request->getWebsite().find(".peer", request->getWebsite().length() - 5) != string::npos) {
				//TODO: make this "static"
				HttpResponse notVerifiable;
				notVerifiable.version = "HTTP/1.0";
				notVerifiable.responseCode = "500";
				notVerifiable.responseText = "Internal Server Error";
				notVerifiable.headers = {
						{"Content-Type",   "text/html"},
						{"Content-Length", to_string(notVerifiableEntity.length())},
						{"Connection",     "keep-alive"}
				};
				notVerifiable.contentLength = notVerifiableEntity.length();
				notVerifiable.content = new char[notVerifiableEntity.length()];
				memcpy(notVerifiable.content, notVerifiableEntity.c_str(), notVerifiable.contentLength);
				connection.sendStr(notVerifiable.compile());
			} else {
				connection.sendStr(request->getObject()->getHttpResponse().compile());
			}
		}
		Log::t("Completed request of url <" + request->getObjectUrl() + "> of site <" + request->getWebsite() + '>');
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
