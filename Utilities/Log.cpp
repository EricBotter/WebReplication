#include "Log.h"
#include <ctime>
#include <sstream>
#include <iostream>

// Log static variables
LogLevel Log::level;
bool Log::logToFile;
ofstream Log::file;

void Log::log(string message, LogLevel level) {
	if (level < Log::level) return;

	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%F %T: ", timeinfo);
	stringstream ss;
	ss << buffer << message << endl;

	if (logToFile) {
		file.open("log.log", ios::out | ios::app);
		file.write(ss.str().c_str(), ss.str().length());
		file.close();
	}
	cout << ss.str();

	cout.flush();
}
