#include "ServerDownloader.h"
#include "../Network/FileVerifier.h"

ServerDownloader::ServerDownloader() {
	downloaderThread = new thread(&ServerDownloader::threadFunction, this);
}

ServerDownloader::~ServerDownloader() {
	enqueueWebsite(nullptr);
	if (downloaderThread->joinable()) {
		downloaderThread->join();
	}
	delete downloaderThread;
}

bool ServerDownloader::enqueueWebsite(const string& website) {
	if (FileVerifier::canVerify(website)) {
		HttpRequest hr;
		hr.version = "HTTP/1.0";
		hr.method = "GET";
		hr.url = "/.webrd";
		hr.headers = {{"Connection", "keep-alive"}, {"Host", website}};

		shared_ptr<VerifiedObjectRequest> temp = make_shared<VerifiedObjectRequest>(hr);
		wd.enqueueRequest(temp);
		requestQueue.push(temp);
		return true;
	}
	return false;
}

void ServerDownloader::threadFunction() {
	shared_ptr<VerifiedObjectRequest> request;
	while ((request = requestQueue.pop())) {
		request->waitForVerification();
		if (request->isVerified()) {
			if (request->getObject()->getHttpRequest().url == "/.webrd") {
				//TODO: enqueue files request
			}
			//TODO: save file to disk
		}
	}
}

