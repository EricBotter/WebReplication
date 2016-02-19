#include <cstdlib>
#include <sstream>
#include "HttpRequest.h"

HttpRequest::HttpRequest(string content) {
	size_t index = content.find("\r\n");
	string currentLine = content.substr(0, index);
	size_t lineIndex = currentLine.find(" ");

	method=currentLine.substr(0, lineIndex);
	url=currentLine.substr(lineIndex+1, lineIndex = currentLine.find(" ", lineIndex+1));
	version=currentLine.substr(lineIndex);

	currentLine = content.substr(index, index = content.find("\r\n", index));
	while (currentLine != "") {
		lineIndex = currentLine.find(": ");
		headers.emplace(currentLine.substr(0, lineIndex), currentLine.substr(lineIndex+2));
		currentLine = content.substr(index, index = content.find("\r\n", index));
	}
}

string HttpRequest::compile() {
	stringstream out;
	out << method << " " << url << " " << version << "\r\n";
	for (auto& it : headers) {
		out << it.first << ": " << it.second << "\r\n";
	}
	out << "\r\n";
	return out.str();
}
