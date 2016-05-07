#include "Resolver.h"

vector<string> Resolver::resolve(const string& website) {
	lock_guard<mutex> guard(addressMutex);
	return vector<string>(addressTable[website].begin(), addressTable[website].end());
}

void Resolver::add(const string& website, const string& host) {
	{
		lock_guard<mutex> guard(addressMutex);
		addressTable[website].insert(host);
	}
	lock_guard<mutex> guard(hostsMutex);
	hosts.insert(host);
}

void Resolver::remove(const string& host) {
	{
		lock_guard<mutex> guard(addressMutex);
		for (auto it = addressTable.begin(); it != addressTable.end(); ++it) {
			it->second.erase(host);
		}
	}
	lock_guard<mutex> guard(hostsMutex);
	hosts.erase(host);
}

set<string> Resolver::getAllHosts() {
	lock_guard<mutex> guard(hostsMutex);
	return hosts;
}
