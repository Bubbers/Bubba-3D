
#ifndef SUPER_BUBBA_AWESOME_SPACE_STDOUTLOGHANDLER_H__
#define SUPER_BUBBA_AWESOME_SPACE_STDOUTLOGHANDLER_H__

#include "LogHandler.h"
#include <string>

class StdOutLogHandler : public LogHandler {
public:
    StdOutLogHandler() {};
    ~StdOutLogHandler() {};

    virtual void log(std::string msg) {
        printf("%s", msg.c_str());
    };

};


#endif //SUPER_BUBBA_AWESOME_SPACE_STDOUTLOGHANDLER_H
