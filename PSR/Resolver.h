#ifndef THESIS_RESOLVER_H
#define THESIS_RESOLVER_H

#include <map>
#include <vector>

using namespace std;

class Resolver {
private:
	map<string, vector<string>> addressTable;

public:
	vector<string> resolve(string);
	void add(string, string);
};


#endif //THESIS_RESOLVER_H
