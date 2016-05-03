#ifndef THESIS_INIREADER_H
#define THESIS_INIREADER_H

#include <map>

using namespace std;

class IniReader {
public:
	map<string, string> properties;
	void readFromFile(const string&);
};


#endif //THESIS_INIREADER_H
