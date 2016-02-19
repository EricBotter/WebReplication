#ifndef THESIS_HTTPRESPONSE_H
#define THESIS_HTTPRESPONSE_H

#include <map>
#include "HttpRequest.h"

using namespace std;

class HttpResponse {
private:
	string version;
	string responseCode;
	string responseText;
	map<string, string> headers;
	size_t contentLength;
	char* content;

public:
	HttpResponse(string);
	~HttpResponse();
	string compile();
};


#endif //THESIS_HTTPRESPONSE_H
