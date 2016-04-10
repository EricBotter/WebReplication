#include "NetworkRequest.h"


NetworkRequest::NetworkRequest(HttpRequest hr) : originalRequest(hr) {
	response = HttpResponse();
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

HttpRequest& NetworkRequest::getHttpRequest() {
	return originalRequest;
}

HttpResponse& NetworkRequest::getHttpResponse() {
	return response;
}

void NetworkRequest::setHttpRequest(const HttpRequest& hr) {
	originalRequest = hr;
}

void NetworkRequest::setHttpResponse(const HttpResponse& hr) {
	response = hr;
}




