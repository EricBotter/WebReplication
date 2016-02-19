#include "Log.h"
#include <ctime>
#include <sstream>
#include <iostream>

void Log::log(string message, LogLevel level) {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (buffer,80,"%F %T.",timeinfo);
	stringstream ss;
	ss << buffer << rawtime%1000 << ": " << message << endl;

	if (logToFile) {
		file.open("log.log", ios::out | ios::app);
		file.write(ss.str().c_str(), ss.str().length());
		file.close();
	}
	cout << ss.str();
}
