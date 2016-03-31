#ifndef THESIS_HTTPREQUEST_H
#define THESIS_HTTPREQUEST_H

#include <map>
#include "../TCP/Connection.h"

using namespace std;

class HttpRequest {
public:
	string method;
	string url;
	string version;
	map<string, string> headers;

	HttpRequest();
	HttpRequest(string);
	string compile();
};


#endif //THESIS_HTTPREQUEST_H
