#include "VerifiedObjectRequest.h"
#include "../Utilities/FileVerifier.h"

VerifiedObjectRequest::VerifiedObjectRequest(shared_ptr<ObjectRequest> objectRequest)
		: object(objectRequest), verified(false), verificationDone(false) {
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
	return FileVerifier::canVerify(object->getHttpRequest().headers["Host"]);
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
