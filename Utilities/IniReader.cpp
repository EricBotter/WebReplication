#include <fstream>
#include <algorithm>
#include <locale>
#include "IniReader.h"
#include "Log.h"

static inline string& ltrim(string& s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	return s;
}

static inline string& rtrim(string& s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}

static inline string& trim(string& s) {
	return ltrim(rtrim(s));
}

void IniReader::readFromFile(const string& filename) {
	ifstream fin(filename);
	if (!fin) {
		return;
	}

	properties.erase(properties.begin(), properties.end());

	int i = 0;
	string line;
	while (getline(fin, line)) {
		++i;
		trim(line);
		if (line.length() == 0 || line[0] == ';' || line[0] == '#' || line[0] == '[')
			continue;
		size_t index = line.find('=');
		if (index == string::npos) {
			Log::w("Error in parsing file <" + filename + "> at line " + to_string(i) + ": line ignored");
		}

		string property = line.substr(0, index);
		string value = line.substr(index + 1);
		properties[trim(property)] = trim(value);
	}
}
