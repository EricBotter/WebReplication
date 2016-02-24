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
	void setSites(const vector<string>&);
	vector<string> getHosts();
	void setMessage(string);

	static uint16_t portFromAddress(string);
	static string addressFromAddress(string);
};


#endif //THESIS_PSRMESSAGE_H
