#include <cstdlib>
#include <sstream>
#include "HttpRequest.h"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(string content) {
	size_t index = content.find("\r\n");
	size_t prevIndex = 0;
	string currentLine = content.substr(prevIndex, index);
	size_t lineIndex = currentLine.find(" ");
	size_t linePrevIndex = 0;

	method=currentLine.substr(linePrevIndex, lineIndex);
	linePrevIndex = lineIndex + 1;
	lineIndex = currentLine.find(" ", linePrevIndex);
	url=currentLine.substr(linePrevIndex, lineIndex-linePrevIndex);
	version=currentLine.substr(lineIndex+1);

	prevIndex = index + 2;
	index = content.find("\r\n", prevIndex);
	currentLine = content.substr(prevIndex, index-prevIndex);
	while (currentLine != "") {
		lineIndex = currentLine.find(": ");
		headers.emplace(currentLine.substr(0, lineIndex), currentLine.substr(lineIndex+2));
		prevIndex = index + 2;
		index = content.find("\r\n", prevIndex);
		currentLine = content.substr(prevIndex, index-prevIndex);
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
