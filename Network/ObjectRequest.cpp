#include "ObjectRequest.h"

ObjectRequest::ObjectRequest(HttpRequest hr) : originalRequest(hr) {
	response = HttpResponse();
	completed = false;
}

bool ObjectRequest::isCompleted() {
	return completed;
}

void ObjectRequest::setCompleted() {
	lock_guard<mutex> guard(completedMutex);
	completed = true;
	completedCV.notify_one();
}

HttpRequest& ObjectRequest::getHttpRequest() {
	return originalRequest;
}

HttpResponse& ObjectRequest::getHttpResponse() {
	return response;
}

//Deprecated
void ObjectRequest::setHttpRequest(const HttpRequest& hr) {
	originalRequest = hr;
}

void ObjectRequest::setHttpResponse(const HttpResponse& hr) {
	response = hr;
}

void ObjectRequest::waitForCompleted() {
	unique_lock<mutex> guard(completedMutex);
	while (!completed)
		completedCV.wait(guard);
}
