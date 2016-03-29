#ifndef THESIS_NETWORKREQUEST_H
#define THESIS_NETWORKREQUEST_H


#include "../HTTP/HttpRequest.h"

class NetworkRequest {
private:
	HttpRequest& originalRequest;
	bool completed;

public:
	NetworkRequest(HttpRequest&);
	// Will update this API when chunks are introduced
	bool isCompleted();
	bool setCompleted(bool);

	HttpRequest& getRequest();
};


#endif //THESIS_NETWORKREQUEST_H
