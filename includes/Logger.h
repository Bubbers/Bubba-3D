#pragma once

#include <string>
using namespace std;

class Logger
{
public:

	~Logger();
	Logger();

	static void setDebug(bool debug);
	static void setFileSave(bool saveToFile);

	static void logDebug(string msg);
	static void logSevere(string msg);
	static void logInfo(string msg);

	static bool debug;
	static bool saveToFile;

private:

	static string getTime();
};

