#include <sstream>
#include "PsrMessage.h"

#define PSR_VERSION "PSR/0.3"

PsrMessage::PsrMessage() {
	version = PSR_VERSION;
}

PsrMessage::PsrMessage(const string& content) {
	string line;

	size_t prevIndex = content.find("\r\n") + 2;
	line = content.substr(0, prevIndex);
	size_t index = line.find(' ');
	version = line.substr(0, index);
	message = line.substr(index + 1, line.length() - index - 3);

	index = content.find("\r\n", prevIndex);
	while (index != string::npos) {
		line = content.substr(prevIndex, index);

		prevIndex = line.find(": ");
		if (prevIndex == string::npos)
			break;
		values.insert({{line.substr(0, prevIndex), line.substr(prevIndex + 2)}});

		prevIndex = index;
		index = content.find("\r\n", prevIndex);
	}
}

PsrMessage::PsrMessage(Connection& connection) {
	string content = connection.receive("\r\n\r\n");
	string line;

	size_t prevIndex = content.find("\r\n") + 2;
	line = content.substr(0, prevIndex);
	size_t index = line.find(' ');
	version = line.substr(0, index);
	message = line.substr(index + 1, line.length() - index - 3);

	index = content.find("\r\n", prevIndex);
	while (index != string::npos) {
		line = content.substr(prevIndex, index - prevIndex);

		prevIndex = line.find(": ");
		if (prevIndex == string::npos)
			break;
		values.insert({{line.substr(0, prevIndex), line.substr(prevIndex + 2)}});

		prevIndex = index + 2;
		index = content.find("\r\n", prevIndex);
	}
}

uint16_t PsrMessage::portFromAddress(const string& address) {
	size_t index = address.find(":");
	if (index == string::npos) {
		return 0;
	}
	stringstream ss;
	uint16_t out;
	ss << address.substr(index + 1);
	ss >> out;
	return out;
}

string PsrMessage::addressFromAddress(const string& address) {
	size_t index = address.find(":");
	if (index == string::npos) {
		return "";
	}
	return address.substr(0, index);
}

string PsrMessage::compile() {
	string out = version + ' ' + message + "\r\n";
	for (auto it = values.begin(); it != values.end(); ++it) {
		out.append(it->first + ": " + it->second + "\r\n");
	}
	out.append("\r\n");
	return out;
}

void PsrMessage::setAddresses(const vector<string>& addresses) {
	version = PSR_VERSION;
	values.erase(values.begin(), values.end());
	if (addresses.size() == 0) {
		message = "NONE";
		return;
	}
	message = "OK";
	stringstream ss;
	for (auto it = addresses.begin(); it != addresses.end(); ++it)
		ss << ' ' << *it;
	values.insert({{"Address", ss.str().substr(1)}});
}

vector<string> PsrMessage::getHosts() {
	vector<string> hosts;
	if (message != "OK" || values.size() != 1 || values.begin()->first != "Address")
		return hosts;

	string value = values.begin()->second;
	size_t prevIndex = 0, index;
	while ((index = value.find(" ", prevIndex)) != string::npos) {
		hosts.push_back(value.substr(prevIndex, index - prevIndex));
		prevIndex = index + 1;
	}
	hosts.push_back(value.substr(prevIndex));
	return hosts;
}

void PsrMessage::setOk() {
	version = PSR_VERSION;
	message = "OK";
	values.erase(values.begin(), values.end());
}

void PsrMessage::setInvalid() {
	version = PSR_VERSION;
	message = "INVALID";
	values.erase(values.begin(), values.end());
}

void PsrMessage::setSites(const vector<string>& sites, const string& host) {
	version = PSR_VERSION;
	message = "ANNOUNCE";
	values.erase(values.begin(), values.end());
	stringstream ss;
	for (string s : sites) {
		ss << " " << s;
	}
	values.insert({{"Available", ss.str().substr(1)},
				   {"Server",    host}});
}

void PsrMessage::setResolve(const string& host) {
	version = PSR_VERSION;
	message = "RESOLVE";
	values.erase(values.begin(), values.end());
	values.insert({{"Host", host}});
}

vector<string> PsrMessage::getAnnounced(string& server) {
	vector<string> sites;
	if (message != "ANNOUNCE" || values.size() != 2 || values.find("Available") == values.end() ||
		values.find("Server") == values.end())
		return sites;

	server = values["Server"];
	string value = values["Available"];
	size_t prevIndex = 0, index;
	while ((index = value.find(" ", prevIndex)) != string::npos) {
		sites.push_back(value.substr(prevIndex, index - prevIndex));
		prevIndex = index + 1;
	}
	sites.push_back(value.substr(prevIndex));
	return sites;
}

void PsrMessage::setReplicate(const vector<string>& websites) {
	version = PSR_VERSION;
	values.erase(values.begin(), values.end());
	message = "OK";
	if (websites.size() != 0) {
		stringstream ss;
		for (auto it = websites.begin(); it != websites.end(); ++it)
			ss << ' ' << *it;
		values.insert({{"Replicate", ss.str().substr(1)}});
	}
}
