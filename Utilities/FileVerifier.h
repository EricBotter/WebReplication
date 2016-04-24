#ifndef THESIS_FILEVERIFIER_H
#define THESIS_FILEVERIFIER_H

#include <string>

using namespace std;

class FileVerifier {
public:
	static void sign(string filepath, string signaturepath);
	static bool verify(const string&, const string&, const string&);
	static bool canVerify(string);
};


#endif //THESIS_FILEVERIFIER_H
