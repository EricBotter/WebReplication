#include <fstream>
#include <algorithm>
#include <locale>
#include "IniReader.h"
#include "Log.h"

static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

static inline std::string &trim(std::string &s) {
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
		if (line[0] == ';' || line[0] == '#' || line[0] == '[')
			continue;
		size_t index = line.find('=');
		if (index == string::npos) {
			Log::w("Error in parsing file <" + filename + "> at line " + to_string(i) + ": line skipped");
		}

		string property = line.substr(0, index);
		string value = line.substr(index + 1);
		properties[trim(property)] = trim(value);
	}
}
