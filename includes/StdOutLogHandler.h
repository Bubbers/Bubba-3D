
#ifndef SUPER_BUBBA_AWESOME_SPACE_STDOUTLOGHANDLER_H
#define SUPER_BUBBA_AWESOME_SPACE_STDOUTLOGHANDLER_H

#include "LogHandler.h"

class StdOutLogHandler : public LogHandler {
public:
    StdOutLogHandler() {};
    ~StdOutLogHandler() {};

    virtual void log(string msg) {
        printf("%s", msg);
    };

};


#endif //SUPER_BUBBA_AWESOME_SPACE_STDOUTLOGHANDLER_H
