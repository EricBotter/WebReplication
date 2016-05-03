#ifndef THESIS_VERIFIEDOBJECTREQUEST_H
#define THESIS_VERIFIEDOBJECTREQUEST_H


#include "ObjectRequest.h"

class VerifiedObjectRequest {
private:
	shared_ptr<ObjectRequest> object;
	shared_ptr<ObjectRequest> signature;
	bool verificationDone;
	bool verified;
public:
	VerifiedObjectRequest(const HttpRequest&);
	VerifiedObjectRequest(shared_ptr<ObjectRequest>);
	shared_ptr<ObjectRequest> getObject();
	shared_ptr<ObjectRequest> getSignature();
	string getWebsite();
	string getObjectUrl();
	bool canBeVerified();
	bool isVerified();
	bool hasFailed();
	bool waitForVerification();
};


#endif //THESIS_VERIFIEDOBJECTREQUEST_H
