#ifndef THESIS_WEBRDREADER_H
#define THESIS_WEBRDREADER_H

#include <string>
#include <vector>

using namespace std;

class WebrdReader {
public:
	string website = "";
	int version = -1;
	vector<string> files;

	void readFromString(const string&);
};


#endif //THESIS_WEBRDREADER_H
