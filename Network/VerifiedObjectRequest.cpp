#include "VerifiedObjectRequest.h"

bool VerifiedObjectRequest::isVerified() {
	if (!verificationDone) {
//		verified = FileVerifer::verify();
		verificationDone = true;
	}
	return verified;
}

