#ifndef THESIS_LOG_H
#define THESIS_LOG_H

#include <fstream>

using namespace std;

enum LogLevel {
	TRACE, DEBUG, WARN, ERROR, FATAL
};

class Log {
private:
	static LogLevel level;
	static bool logToFile;
	static ofstream file;

public:
	static void setLogLevel(LogLevel level) {
		Log::level = level;
	}
	static void log(const string&, LogLevel);
	static inline void t(const string& message) {
		log(message, TRACE);
	}
	static inline void d(const string& message) {
		log(message, DEBUG);
	}
	static inline void w(const string& message) {
		log(message, WARN);
	}
	static inline void e(const string& message) {
		log(message, ERROR);
	}
	static inline void f(const string& message) {
		log(message, FATAL);
	}
};


#endif //THESIS_LOG_H
