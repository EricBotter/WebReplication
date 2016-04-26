#ifndef THESIS_OBJECTREQUEST_H
#define THESIS_OBJECTREQUEST_H

#include <mutex>
#include <condition_variable>
#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"

using namespace std;

class ObjectRequest {
private:
	HttpRequest originalRequest;
	HttpResponse response;
	bool completed;
	mutex completedMutex;
	condition_variable completedCV;

public:
	ObjectRequest(HttpRequest);
	// Will update this API when chunks are introduced
	bool isCompleted();
	void setCompleted();
	void waitForCompleted();

	// This API is for the Proxy - will not change.
	HttpRequest& getHttpRequest();
	HttpResponse& getHttpResponse();
	//Deprecated -- original request shouldn't change
	void setHttpRequest(const HttpRequest&);
	void setHttpResponse(const HttpResponse&);
};


#endif //THESIS_OBJECTREQUEST_H
