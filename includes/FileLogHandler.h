
#ifndef SUPER_BUBBA_AWESOME_SPACE_STDOUTLOGHANDLER_H
#define SUPER_BUBBA_AWESOME_SPACE_STDOUTLOGHANDLER_H

#include "LogHandler.h"
#include <fstream>
#include <string>
using namespace std;

class FileLogHandler : public LogHandler {
public:
    FileLogHandler(string fileName) {
        logFile.open(fileName.c_str());
    };
    ~FileLogHandler() {
        logFile.close();
    };

    virtual void log(string msg) {
        logFile << msg;
    };

private:
    ofstream logFile;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_STDOUTLOGHANDLER_H
