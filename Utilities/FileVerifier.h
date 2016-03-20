#ifndef THESIS_FILEVERIFIER_H
#define THESIS_FILEVERIFIER_H

#include <string>

using namespace std;

class FileVerifier {
public:
	static void sign(string filepath, string signaturepath);
	static bool verify(string filepath, string signaturepath);
};


#endif //THESIS_FILEVERIFIER_H
