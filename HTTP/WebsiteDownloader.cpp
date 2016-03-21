#include "WebsiteDownloader.h"
#include "../PSR/PsrMessage.h"

#define N_THREADS 8

WebsiteDownloader::WebsiteDownloader() {
	threads.reserve(N_THREADS);
	for (int i = 0; i < N_THREADS; ++i) {
		threads.push_back(thread(threadFunction));
	}
}

WebsiteDownloader::~WebsiteDownloader() {
	active = false;
	for (int i = 0; i < N_THREADS; ++i) {
		threads[i].join();
	}
}

vector<string> WebsiteDownloader::resolve(string hostname) {
	vector<string> hosts;
	map<string, vector<string>>::iterator it;

	resolutionsMutex.lock();
	it = resolutions.find(hostname);
	resolutionsMutex.unlock();

	if (it != resolutions.end()) {
		hosts = it->second;
	} else {
		PsrMessage psrRequest;
		psrRequest.key = "Host";
		psrRequest.value = hostname;

		//FIXME: hardcoded resolution IP and PORT
		Connection psrConn("127.0.0.1", 3921);
		psrConn.sendStr(psrRequest.compile());
		PsrMessage psrResponse(psrConn);
		hosts = psrResponse.getHosts();

		resolutionsMutex.lock();
		resolutions.insert({hostname, hosts});
		resolutionsMutex.unlock();
	}

	return hosts;
}

void WebsiteDownloader::threadFunction() {
	string currentHost;
	Connection* currentConn = NULL;
	unique_lock<mutex> lock(requestQueueMutex);
	while (active) {
		requestQueueCV.wait(lock);
		if (requestQueue.empty()) {
			continue;
		}
		auto url = requestQueue.front();
		requestQueue.pop();
		requestQueueMutex.unlock();
		requestQueueCV.notify_one();

		if (url.first != currentHost) {
			if (currentConn != NULL)
				delete currentConn;
			vector<string> hosts = resolve(url.first);
			string randomHost = hosts[rand() % hosts.size()];

			currentConn = new Connection(PsrMessage::addressFromAddress(randomHost),
										 PsrMessage::portFromAddress(randomHost));
		}

		HttpRequest request;
		request.version = "HTTP/1.0";
		request.url = url.second;
		request.headers = {{"Connection", "keep-alive"}, {"Host", url.first}};

		currentConn->sendStr(request.compile());

		HttpResponse response(*currentConn);
		if (response.responseCode == "200") {
			webpageCacheMutex.lock();
			webpageCache.insert({url, string(response.content, response.contentLength)});
			webpageCacheMutex.unlock();
			webpageCacheCV.notify_one();
		} else {
			//TODO: figure out a way to signal error
		}
	}
}

void WebsiteDownloader::setActiveCaching(bool active) {
	//TODO: prefetching of possible required objects
}

HttpResponse WebsiteDownloader::getWebpage(string host, string page) {
	unique_lock<mutex> lock(requestQueueMutex);
	requestQueue.push(make_pair(host, page));
	lock.unlock();
	requestQueueCV.notify_one();
	unique_lock<mutex> pageLock(webpageCacheMutex);
	webpageCacheCV.wait(pageLock);

	return HttpResponse();
}
