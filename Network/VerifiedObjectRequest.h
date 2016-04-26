#ifndef THESIS_VERIFIEDOBJECTREQUEST_H
#define THESIS_VERIFIEDOBJECTREQUEST_H


#include "ObjectRequest.h"

class VerifiedObjectRequest {
private:
	ObjectRequest object;
	ObjectRequest signature;
	bool verificationDone;
	bool verified;
public:
	bool canBeVerified();
	bool isVerified();

};


#endif //THESIS_VERIFIEDOBJECTREQUEST_H
