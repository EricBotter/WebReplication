#ifndef THESIS_HTTPREQUEST_H
#define THESIS_HTTPREQUEST_H

#include <map>

using namespace std;

class HttpRequest {
private:
	string method;
	string url;
	string version;
	map<string, string> headers;

public:
	HttpRequest(string);
	virtual string compile();
};


#endif //THESIS_HTTPREQUEST_H
