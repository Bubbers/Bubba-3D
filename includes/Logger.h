
#ifndef __LOGGER__
#define __LOGGER__

#include <vector>
#include <string>
#include "LogHandler.h"

using namespace std;

enum LogLevel {DEBUG, INFO, WARNING, SEVERE};

class Logger
{
public:

	static void setDebug(bool debug);
	static void setFileSave(bool saveToFile);

	static void logDebug(string msg);
	static void logError(string msg);
	static void logWarning(string msg);
	static void logInfo(string msg);

	static void setLogLevel(unsigned int level);
	static void addLogHandler(LogHandler* logHandler);

private:
	~Logger();
	Logger();

	static unsigned int currentLogLevel;
    static std::vector<LogHandler*> logHandlers;
	static string levelToString(unsigned int level);
	static void log(unsigned int level, string msg);
	static string getTime();
};

#endif