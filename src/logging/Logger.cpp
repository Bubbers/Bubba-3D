#include "Logger.h"

#include<time.h>
#include<cstdio>

using namespace std;


bool Logger::debug      = true;
bool Logger::saveToFile = false;

void Logger::logDebug(string msg) {
	if (debug) {
	  printf("[DEBUG] %s %s\n", getTime().c_str(), msg.c_str());
	}
}

void Logger::logSevere(string msg) {
	printf("[SEVERE] %s %s\n", getTime().c_str(), msg.c_str());
}

void Logger::logInfo(string msg) {
	printf("[INFO] %s  %s\n", getTime().c_str(), msg.c_str());
}

string Logger::getTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}
