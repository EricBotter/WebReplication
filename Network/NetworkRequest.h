#ifndef THESIS_NETWORKREQUEST_H
#define THESIS_NETWORKREQUEST_H


#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"

class NetworkRequest {
private:
	HttpRequest originalRequest;
	HttpResponse response;
	bool completed;

public:
	NetworkRequest(HttpRequest&);
	// Will update this API when chunks are introduced
	bool isCompleted();
	bool setCompleted(bool);

	// This API is for the Proxy - will not change.
	HttpRequest& getRequest();
	HttpResponse& getResponse();
};


#endif //THESIS_NETWORKREQUEST_H
