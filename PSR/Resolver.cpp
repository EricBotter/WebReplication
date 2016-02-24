#include "Resolver.h"

vector<string> Resolver::resolve(string host) {
/*	vector<string> out;
	for (auto it = addressTable[host].begin(); it != addressTable[host].end(); it++) {
		out.push_back(*it);
	}
	return out;*/
	return vector<string>(addressTable[host].begin(), addressTable[host].end());
}

void Resolver::add(string host, string address) {
	addressTable[host].insert(address);
}
