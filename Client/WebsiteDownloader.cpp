#include "WebsiteDownloader.h"
#include "../PSR/PsrMessage.h"
#include "../Utilities/Log.h"

#define N_THREADS 4


WebsiteDownloader::WebsiteDownloader() : resolverAddress("127.0.0.1:3921") {
	threads.reserve(N_THREADS);
	for (int i = 0; i < N_THREADS; ++i) {
		threads.push_back(thread(&WebsiteDownloader::threadFunction, this));
	}
}

WebsiteDownloader::~WebsiteDownloader() {
	active = false;
	for (int i = 0; i < N_THREADS; ++i) {
		if (threads[i].joinable())
			threads[i].join();
	}
}

vector<string> WebsiteDownloader::resolve(const string& hostname) {
	if (hostname.find(".peer") == string::npos)
		//FIXME: horrendous hack
		return {hostname + ":80"};

	vector<string> hosts;
	map<string, vector<string>>::iterator it;

	resolutionsMutex.lock();
	it = resolutionCache.find(hostname);
	resolutionsMutex.unlock();

	if (it != resolutionCache.end()) {
		hosts = it->second;
	} else {
		PsrMessage psrRequest;
		psrRequest.setResolve(hostname);
		Connection psrConn(
				PsrMessage::addressFromAddress(resolverAddress),
				PsrMessage::portFromAddress(resolverAddress)
		);
		psrConn.sendStr(psrRequest.compile());
		PsrMessage psrResponse(psrConn);
		hosts = psrResponse.getHosts();

		resolutionsMutex.lock();
		resolutionCache.insert({hostname, hosts});
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
		serverFromWebsite(website)->enqueueRequest(request->getObject());
		if (request->canBeVerified())
			serverFromWebsite(website)->enqueueRequest(request->getSignature());
		Log::t("Sent requests for url <" + request->getWebsite() + request->getObjectUrl() + '>');
	}
}

void WebsiteDownloader::enqueueRequest(shared_ptr<VerifiedObjectRequest> request) {
	requestQueue.push(request);
}

shared_ptr<HttpClientConnection> WebsiteDownloader::serverFromWebsite(const string& website) {
	shared_ptr<HttpClientConnection> bestConnection = nullptr;
	while (bestConnection == nullptr) {
		vector<string> resolutions = resolve(website);
		vector<shared_ptr<HttpClientConnection>> existingConnections;

		// check for non-connected servers -- also collect existingConnections
		connectionsMutex.lock();
		for (auto it = resolutions.begin(); it != resolutions.end(); ++it) {
			auto connection = connections.find(*it);
			if (connection != connections.end()) {
				if (connection->second->isActive())
					existingConnections.push_back(connection->second);
				else {
					connection->second->join();
					connections.erase(connection);
				}
			} else {
				bestConnection = make_shared<HttpClientConnection>(
						PsrMessage::addressFromAddress(*it),
						PsrMessage::portFromAddress(*it)
				);
				if (bestConnection->isActive()) {
					connections.insert({{*it, bestConnection}});
					bestConnection->run();
					connectionsMutex.unlock();
					return bestConnection;
				} else {
					bestConnection = nullptr;
					// invalidate cache -- connection to resolved server failed
					resolutionsMutex.lock();
					resolutionCache.erase(resolutionCache.begin(), resolutionCache.end());
					resolutionsMutex.unlock();
				}
			}
		}
		connectionsMutex.unlock();

		// if all servers are already connected, take the one with the least elements in queue
		if (existingConnections.size() != 0) {
			bestConnection = *existingConnections.begin();
			size_t min = bestConnection->queueLength();
			for (auto it = existingConnections.begin() + 1; it != existingConnections.end(); ++it) {
				if ((*it)->queueLength() < min) {
					bestConnection = *it;
					min = bestConnection->queueLength();
				}
			}
		} else {
			// if no suitable servers are found, then invalidate cache, wait 5sec for resolver to update and try again
			resolutionsMutex.lock();
			resolutionCache.erase(resolutionCache.begin(), resolutionCache.end());
			resolutionsMutex.unlock();
			Log::w("Unable to find a server hosting <" + website + ">, trying again in 2 seconds");
			this_thread::sleep_for(chrono::seconds(2));
		}
	}
	return bestConnection;
}

void WebsiteDownloader::setResolverAddress(const string& newAddress) {
	resolverAddress = newAddress;
}
