#include "Resolver.h"

vector<string> Resolver::resolve(string host) {
	return addressTable[host];
}

void Resolver::add(string host, string address) {
	addressTable[host].push_back(address);
}
