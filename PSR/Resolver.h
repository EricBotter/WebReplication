#ifndef THESIS_RESOLVER_H
#define THESIS_RESOLVER_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include <mutex>

using namespace std;

class Resolver {
private:
	map<string, set<string>> addressTable;
	set<string> hosts;
	mutex addressMutex;
	mutex hostsMutex;

public:
	vector<string> resolve(const string&);
	void add(const string&, const string&);
	void remove(const string&);
	set<string> getAllHosts();
	vector<string> getAllWebsites();
};


#endif //THESIS_RESOLVER_H
