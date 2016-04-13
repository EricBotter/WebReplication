#include "NetworkRequest.h"


NetworkRequest::NetworkRequest(HttpRequest hr) : originalRequest(hr) {
	response = HttpResponse();
	completed = false;
}

bool NetworkRequest::isCompleted() {
	return completed;
}

void NetworkRequest::setCompleted() {
	lock_guard<mutex> guard(completedMutex);
	completed = true;
	completedCV.notify_one();
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

void NetworkRequest::waitForCompleted() {
	unique_lock<mutex> guard(completedMutex);
	while (!completed)
		completedCV.wait(guard);
}

