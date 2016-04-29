#include "WebsiteDownloader.h"
#include "../PSR/PsrMessage.h"
#include "../Utilities/Log.h"

#define N_THREADS 4

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
	while (active) {
		shared_ptr<VerifiedObjectRequest> request = requestQueue.pop();
		if (request == NULL) {
			active = false;
			break;
		}
		string website = request->getWebsite();
		vector<string> resolutions = resolve(website);
		randomServerFromList(resolutions)->enqueueRequest(request->getObject());
		randomServerFromList(resolutions)->enqueueRequest(request->getSignature());
		Log::t("Sent requests for url <" + request->getWebsite() + request->getObjectUrl() + '>');
		connectionsMutex.unlock();
	}
}

void WebsiteDownloader::setActiveCaching(bool active) {
	//TODO: prefetching of possible required objects
}

void WebsiteDownloader::enqueueRequest(shared_ptr<VerifiedObjectRequest> request) {
	requestQueue.push(request);
}

HttpConnection* WebsiteDownloader::randomServerFromList(const vector<string>& resolutions) {
	string randomServer = resolutions[rand() % resolutions.size()];
	connectionsMutex.lock();
	if (connections.find(randomServer) == connections.end()) {
		connections.insert({randomServer, new HttpConnection(
				PsrMessage::addressFromAddress(randomServer),
				PsrMessage::portFromAddress(randomServer)
		)});
		connections[randomServer]->run();
	}
	connectionsMutex.unlock();
	return connections[randomServer];
}




