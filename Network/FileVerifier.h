#ifndef THESIS_FILEVERIFIER_H
#define THESIS_FILEVERIFIER_H

#include <string>

using namespace std;

class FileVerifier {
public:
	static bool verify(const string&, const string&, const string&);
	static bool canVerify(const string&);
};


#endif //THESIS_FILEVERIFIER_H
