
#ifndef __LOGGER__
#define __LOGGER__

#include <vector>
#include <string>
#include "LogHandler.h"

using namespace std;

class Logger
{
public:

	static void setDebug(bool debug);
	static void setFileSave(bool saveToFile);

	static void logDebug(string msg);
	static void logError(string msg);
	static void logWarning(string msg);
	static void logInfo(string msg);

	static const unsigned int DEBUG = 0;
	static const unsigned int INFO = 1;
	static const unsigned int WARNING = 2;
	static const unsigned int ERROR = 4;

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