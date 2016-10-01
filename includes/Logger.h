/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */

#ifndef __LOGGER__
#define __LOGGER__

#include <vector>
#include <string>
#include "LogHandler.h"

using namespace std;

enum LogLevel {INFO, DEBUG, WARNING, SEVERE};

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