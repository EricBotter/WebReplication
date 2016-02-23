#include <sstream>
#include "PsrMessage.h"

PsrMessage::PsrMessage() {
	version = "PSR/0.1";
}

PsrMessage::PsrMessage(string content) {
	size_t index = content.find("\r\n") + 2;
	version = content.substr(0, index);
	size_t prevIndex = index;
	index = content.find("\r\n");
	content = content.substr(prevIndex, index - prevIndex);

	index = content.find(": ");
	key = content.substr(0, index);
	value = content.substr(index + 2);
}

PsrMessage::PsrMessage(Connection& connection) {
	string content = connection.receive("\r\n\r\n");

	size_t index = content.find("\r\n") + 2;
	version = content.substr(0, index);
	size_t prevIndex = index;
	index = content.find("\r\n");
	content = content.substr(prevIndex, index - prevIndex);

	index = content.find(": ");
	key = content.substr(0, index);
	value = content.substr(index + 2);
}

int PsrMessage::portFromAddress(string address) const {
	size_t index = address.find(":");
	if (index == string::npos) {
		return -1;
	}
	stringstream ss;
	int out;
	ss << address.substr(index + 1);
	ss >> out;
	return out;
}

string PsrMessage::addressFromAddress(string address) const {
	size_t index = address.find(":");
	if (index == string::npos) {
		return "";
	}
	return address.substr(0, index);
}

string PsrMessage::compile() {
	return version + "\r\n" + key + ": " + value + "\r\n\r\n";
}

void PsrMessage::setAddresses(const vector<string>& addresses) {
	key = "Address";
	stringstream ss;
	for (string s : addresses) {
		ss << " " << s;
	}
	value = ss.str().substr(1);
}

vector PsrMessage::getHosts() {
	vector<string> hosts;
	size_t prevIndex = 0, index = value.find(" ");
	while (index != string::npos) {
		hosts.push_back(value.substr(prevIndex, index-prevIndex));
		prevIndex = index + 1;
		index = value.find(" ", prevIndex);
	}
	return hosts;
}

void PsrMessage::setMessage(string message) {
	key = "Message";
	value = message;
}
