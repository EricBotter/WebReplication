#include "Log.h"
#include <ctime>
#include <sstream>
#include <iostream>

// Log static variables
LogLevel Log::level = LogLevel::WARN;
bool Log::logToFile = false;
bool Log::logToProgramFlag = false;
ofstream Log::file;
mutex Log::cerrMtx;

void Log::log(const string& message, LogLevel level) {
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

void Log::logToProgram(const string& message) {
	if (!logToProgramFlag) return;

	lock_guard<mutex> guard(cerrMtx);
	cerr << message << endl;
	cerr.flush();
}

void Log::enableLogToProgram() {
	logToProgramFlag = true;
}
