//
// Created by Erc on 2/19/2016.
//

#ifndef THESIS_LOG_H
#define THESIS_LOG_H

#include <fstream>

using namespace std;

enum LogLevel {
	INFO, DEBUG, WARN, ERROR, FATAL
};

class Log {
private:
	static LogLevel level;
	static bool logToFile;
	static ofstream file;

public:
	static void log(string, LogLevel);
	static void i(string message) {
		log(message, INFO);
	}
	static void d(string message) {
		log(message, DEBUG);
	}
	static void w(string message) {
		log(message, WARN);
	}
	static void e(string message) {
		log(message, ERROR);
	}
	static void f(string message) {
		log(message, FATAL);
	}
};


#endif //THESIS_LOG_H
