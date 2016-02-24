#ifndef THESIS_RESOLVER_H
#define THESIS_RESOLVER_H

#include <map>
#include <vector>
#include <set>

using namespace std;

class Resolver {
private:
	map<string, set<string>> addressTable;

public:
	vector<string> resolve(string);
	void add(string, string);
};


#endif //THESIS_RESOLVER_H
