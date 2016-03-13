#ifndef SUPER_BUBBA_AWESOME_SPACE_LOGHANDLER_H
#define SUPER_BUBBA_AWESOME_SPACE_LOGHANDLER_H

#include <string>

using namespace std;

class LogHandler {
public:
    LogHandler() { };
    ~LogHandler() { };

    virtual void log(string msg) = 0;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_LOGHANDLER_H
