#include <sstream>
#include <cstdlib>
#include <cstring>
#include "HttpResponse.h"

HttpResponse::HttpResponse(string content) {
	size_t index = content.find("\r\n");
	string currentLine = content.substr(0, index);
	size_t lineIndex = currentLine.find(" ");

	version=currentLine.substr(0, lineIndex);
	responseCode = currentLine.substr(lineIndex+1, lineIndex = currentLine.find(" ", lineIndex+1));
	responseText = currentLine.substr(lineIndex);

	currentLine = content.substr(index, index = content.find("\r\n", index));
	while (currentLine != "") {
		lineIndex = currentLine.find(": ");
		headers.insert(currentLine.substr(0, lineIndex), currentLine.substr(lineIndex+2));
		currentLine = content.substr(index, index = content.find("\r\n", index));
	}

	index += 2;

	contentLength = (size_t) atoi(headers.at("Content-Length").c_str());
	this->content = new char[sizeof(char)*contentLength];
	memcpy(this->content, content.c_str()+index, sizeof(char)*contentLength);
}

string HttpResponse::compile() {
	stringstream out;
	out << version << " " << responseCode << " " << responseText << "\r\n";
	for (auto& it : headers) {
		out << it.first << ": " << it.second << "\r\n";
	}
	out << "\r\n";

	string response = out.str();
	response.append(content, contentLength);
	return response;
}

HttpResponse::~HttpResponse() {
	delete[] this->content;
}
