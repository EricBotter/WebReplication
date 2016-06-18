#ifndef THESIS_PSRMESSAGE_H
#define THESIS_PSRMESSAGE_H

#include <string>
#include <vector>
#include <map>
#include "../TCP/Connection.h"

using namespace std;

class PsrMessage {
private:
	string version;

public:
	string message;
	map<string, string> values;

	PsrMessage();
	PsrMessage(const string&);
	PsrMessage(Connection&);
	string compile();

	vector<string> getHosts();
	vector<string> getAnnounced(string&);

	void setResolve(const string&);
	void setAddresses(const vector<string>&);
	void setSites(const vector<string>&, const string&);
	void setReplicate(const vector<string>&);
	void setOk();
	void setInvalid();

	static uint16_t portFromAddress(const string&);
	static string addressFromAddress(const string&);
};


#endif //THESIS_PSRMESSAGE_H
