#include "NetworkRequest.h"


NetworkRequest::NetworkRequest(HttpRequest& hr) : originalRequest(hr) {
	completed = false;
}

bool NetworkRequest::isCompleted() {
	return completed;
}

bool NetworkRequest::setCompleted(bool b) {
	bool temp = completed;
	completed = b;
	return temp;
}

HttpRequest& NetworkRequest::getRequest() {
	return originalRequest;
}







