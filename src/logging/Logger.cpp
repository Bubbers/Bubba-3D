#include "Logger.h"
#include "sstream"

using namespace std;

unsigned int Logger::currentLogLevel = DEBUG;
std::vector<LogHandler*> Logger::logHandlers;

void Logger::logDebug(string msg) {
	log(DEBUG, msg);
}

void Logger::logWarning(string msg) {
	log(WARNING, msg);
}

void Logger::logError(string msg) {
	log(ERROR, msg);
}

void Logger::logInfo(string msg) {
	log(INFO, msg);
}

void Logger::log(unsigned int level, string msg) {
	if (level >= currentLogLevel) {
		for (auto logHandler : logHandlers) {
			std::stringstream ss;
			ss << levelToString(level) << "\t" << getTime().c_str() << "\t" << msg.c_str() << "\n";
			logHandler->log(ss.str());
		}
	}
}

void Logger::addLogHandler(LogHandler *logHandler) {
    logHandlers.push_back(logHandler);
}

void Logger::setLogLevel(unsigned int level) {
	currentLogLevel = level;
}

string Logger::levelToString(unsigned int level) {
	switch (level) {
		case INFO:    return "INFO";
		case DEBUG:   return "DEBUG";
		case WARNING: return "WARNING";
		case ERROR:   return "ERROR";
		default: return "BAD level";
	}
}

string Logger::getTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}
