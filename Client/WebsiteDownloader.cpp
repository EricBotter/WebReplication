#include "WebsiteDownloader.h"
#include "../PSR/PsrMessage.h"

#define N_THREADS 8

WebsiteDownloader::WebsiteDownloader() {
	threads.reserve(N_THREADS);
	for (int i = 0; i < N_THREADS; ++i) {
		threads.push_back(thread(&WebsiteDownloader::threadFunction, this));
	}
}

WebsiteDownloader::~WebsiteDownloader() {
	active = false;
	for (int i = 0; i < N_THREADS; ++i) {
		threads[i].join();
	}
}

vector<string> WebsiteDownloader::resolve(string hostname) {
	if (hostname.find(".peer") == string::npos)
		//FIXME: horrendous hack
		return {hostname + ":80"};

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
	while (active) {
		Lockable<NetworkRequest>* requestLockable = requestQueue.pop();
		if (requestLockable == NULL) {
			active = false;
			break;
		}
		unique_lock<mutex> guard(requestLockable->getMutex());
		NetworkRequest& request = requestLockable->getObject();

		string host = request.getHttpRequest().headers["Host"];
		if (host != currentHost) {
			if (currentConn != NULL)
				delete currentConn;
			vector<string> hosts = resolve(host);
			string randomHost = hosts[rand() % hosts.size()];

			currentConn = new Connection(PsrMessage::addressFromAddress(randomHost),
										 PsrMessage::portFromAddress(randomHost));
		}

		HttpRequest requestToSend;
		requestToSend.method = "GET";
		requestToSend.version = "HTTP/1.0";
		requestToSend.url = request.getHttpRequest().url;
		requestToSend.headers = {{"Connection", "keep-alive"}, {"Host", host}};

		currentConn->sendStr(requestToSend.compile());

		HttpResponse response(*currentConn);
		request.getHttpResponse() = response; //omg ugly
		request.setCompleted(true);
		requestLockable->getCV().notify_one();
	}
}

void WebsiteDownloader::setActiveCaching(bool active) {
	//TODO: prefetching of possible required objects
}

void WebsiteDownloader::enqueueRequest(Lockable<NetworkRequest>* request) {
	requestQueue.push(request);
}


