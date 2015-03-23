#pragma once

#include <string>
using namespace std;

class Logger
{
public:
	Logger();
	~Logger();

	void setDebug(bool debug);
	void setFileSave(bool saveToFile);

	void logDebug(string msg);
	void logSevere(string msg);
	void logInfo(string msg);

private:
	bool debug;
	bool saveToFile;

	string getTime();
};

