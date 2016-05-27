#include "WebrdReader.h"
#include <sstream>

#define WEBRD_VERSION "WEBRD/0.1"

void WebrdReader::readFromString(const string& content) {
	stringstream ss(content);
	string line;
	int progress = 0;
	while (getline(ss, line)) {
		if (progress == 0) {
			if (line != WEBRD_VERSION)
				return;
			progress = 1;
		} else if (progress == 1) {
			if (line.find("website=") == 0) {
				website = line.substr(string("website=").length());
			} else if (line.find("version=") == 0) {
				version = stoi(line.substr(string("version=").length()));
			} else if (line == "FILES") {
				return;
			}
			if (website != "" && version > 0)
				progress = 2;
		} else if (progress == 2) {
			if (line != "FILES")
				return;
			progress = 3;
		} else if (progress == 3) {
			if (line[0] == '/')
				files.push_back(line);
			else
				return;
		}
	}
}

