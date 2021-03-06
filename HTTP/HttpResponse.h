#ifndef THESIS_HTTPRESPONSE_H
#define THESIS_HTTPRESPONSE_H

#include <map>
#include "HttpRequest.h"
#include "../TCP/Connection.h"

using namespace std;

class HttpResponse {
public:
	string version;
	string responseCode;
	string responseText;
	map<string, string> headers;
	size_t contentLength;
	char* content;

	HttpResponse();
	HttpResponse(const HttpResponse&);
//	HttpResponse(const string& responseCode);
	//TODO: replace this with a method
	HttpResponse(Connection&);
	HttpResponse& operator=(const HttpResponse&);
	~HttpResponse();
	string compile();
};


#endif //THESIS_HTTPRESPONSE_H
