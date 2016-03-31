#include <sstream>
#include <cstdlib>
#include <cstring>
#include "HttpResponse.h"

HttpResponse::HttpResponse() {
	content = NULL;
	contentLength = 0;
}

HttpResponse::HttpResponse(const HttpResponse& hr)
		: version(hr.version), responseCode(hr.responseCode), responseText(hr.responseText),
		  headers(hr.headers), contentLength(hr.contentLength) {
	content = new char[contentLength];
	memcpy(content, hr.content, contentLength);
}

HttpResponse& HttpResponse::operator=(const HttpResponse& hr) {
	version = hr.version;
	responseCode = hr.responseCode;
	responseText = hr.responseText;
	headers = hr.headers;
	contentLength = hr.contentLength;
	content = new char[contentLength];
	memcpy(content, hr.content, contentLength);
	return *this;
}


string HttpResponse::compile() {
	stringstream out;
	out << version << " " << responseCode << " " << responseText << "\r\n";
	for (auto& it : headers) {
		out << it.first << ": " << it.second << "\r\n";
	}
	out << "\r\n";

	string response = out.str();
	if (content != NULL) {
		response.append(content, contentLength);
	}
	return response;
}

HttpResponse::HttpResponse(Connection& connection) {
	string content = connection.receive("\r\n\r\n");

	size_t index = content.find("\r\n");
	size_t prevIndex = 0;
	string currentLine = content.substr(0, index);
	size_t lineIndex = currentLine.find(" ");
	size_t linePrevIndex = 0;

	version = currentLine.substr(linePrevIndex, lineIndex);
	linePrevIndex = lineIndex + 1;
	lineIndex = currentLine.find(" ", linePrevIndex);
	responseCode = currentLine.substr(linePrevIndex, lineIndex - linePrevIndex);
	responseText = currentLine.substr(lineIndex + 1);

	prevIndex = index + 2;
	index = content.find("\r\n", prevIndex);
	currentLine = content.substr(prevIndex, index - prevIndex);
	while (currentLine != "") {
		lineIndex = currentLine.find(": ");
		headers.emplace(currentLine.substr(0, lineIndex), currentLine.substr(lineIndex + 2));
		prevIndex = index + 2;
		index = content.find("\r\n", prevIndex);
		currentLine = content.substr(prevIndex, index - prevIndex);
	}

	auto it = headers.find("Content-Length");
	if (it != headers.end()) {
		contentLength = (size_t)atoi(it->second.c_str());
		this->content = new char[sizeof(char) * contentLength];

		content = connection.receive(contentLength);
		memcpy(this->content, content.c_str(), sizeof(char) * contentLength);
	} else {
		this->content = NULL;
	}
}

HttpResponse::~HttpResponse() {
	if (content != NULL) {
		delete[] content;
	}
}
