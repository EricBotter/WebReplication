#include "VerifiedObjectRequest.h"
#include "FileVerifier.h"

#define MAX_RETRIES 5

VerifiedObjectRequest::VerifiedObjectRequest(const HttpRequest& request)
		: verified(false), verificationDone(false), retries(0) {
	object = make_shared<ObjectRequest>(request);
	signature = make_shared<ObjectRequest>(request);
	signature->getHttpRequest().url.append("?sig");
}

VerifiedObjectRequest::VerifiedObjectRequest(shared_ptr<ObjectRequest> objectRequest)
		: object(objectRequest), verified(false), verificationDone(false), retries(0) {
	signature = make_shared<ObjectRequest>(objectRequest->getHttpRequest());
	signature->getHttpRequest().url.append("?sig");
}

bool VerifiedObjectRequest::isVerified() {
	if (!verificationDone) {
		return false;
	}
	return verified;
}

bool VerifiedObjectRequest::canBeVerified() {
	return getWebsite().find(".peer", getWebsite().length() - 5) != string::npos && FileVerifier::canVerify(getWebsite());
}

bool VerifiedObjectRequest::waitForVerification() {
	object->waitForCompleted();
	if (canBeVerified()) {
		signature->waitForCompleted();
		verified = FileVerifier::verify(
				object->getHttpRequest().headers["Host"],
				string(object->getHttpResponse().content, object->getHttpResponse().contentLength),
				string(signature->getHttpResponse().content, signature->getHttpResponse().contentLength)
		);
		return (verificationDone = true);
	}
	return (verificationDone = false);
}

shared_ptr<ObjectRequest> VerifiedObjectRequest::getObject() {
	return object;
}

shared_ptr<ObjectRequest> VerifiedObjectRequest::getSignature() {
	return signature;
}

string VerifiedObjectRequest::getWebsite() {
	return object->getHttpRequest().headers["Host"];
}

string VerifiedObjectRequest::getObjectUrl() {
	return object->getHttpRequest().url;
}

bool VerifiedObjectRequest::hasFailed() {
	return object->hasFailed() || signature->hasFailed();
}

bool VerifiedObjectRequest::retryThisRequest() {
	if (++retries >= MAX_RETRIES)
		return false;
	object = make_shared<ObjectRequest>(this->object->getHttpRequest());
	signature = make_shared<ObjectRequest>(this->signature->getHttpRequest());
	return true;
}
