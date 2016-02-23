#ifndef THESIS_PSRMESSAGE_H
#define THESIS_PSRMESSAGE_H

#include <string>
#include <vector>
#include "../TCP/Connection.h"

using namespace std;

class PsrMessage {
public:
	string version;
	string key;
	string value;

	PsrMessage();
	PsrMessage(string);
	PsrMessage(Connection&);
	string compile();
	void setAddresses(const vector<string>&);
	vector getHosts();
	int portFromAddress(string) const;
	string addressFromAddress(string) const;
	void setMessage(string);
};


#endif //THESIS_PSRMESSAGE_H
