#ifndef THESIS_NETWORKREQUEST_H
#define THESIS_NETWORKREQUEST_H


#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"
#include "../Utilities/Lockable.h"

class NetworkRequest {
private:
	HttpRequest originalRequest;
	HttpResponse response;
	bool completed;
	mutex completedMutex;
	condition_variable completedCV;

public:
	NetworkRequest(HttpRequest);
	// Will update this API when chunks are introduced
	bool isCompleted();
	void setCompleted();
	void waitForCompleted();

	// This API is for the Proxy - will not change.
	HttpRequest& getHttpRequest();
	HttpResponse& getHttpResponse();
	void setHttpRequest(const HttpRequest&);
	void setHttpResponse(const HttpResponse&);
};


#endif //THESIS_NETWORKREQUEST_H
