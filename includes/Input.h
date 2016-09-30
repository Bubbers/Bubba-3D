#pragma once

#include <ControlStatus.h>

/**
 * Base class for classes that supply the status
 * for bindings to InputManager. Any class that wants
 * to work as an input binding must extend this class.
 */
class Input {
public:
    virtual ~Input() = 0;
    virtual ControlStatus getStatus() = 0;
    virtual ControlStatus::Activator getActivator() = 0;
    bool isDual();

protected:
    Input(bool dual);
    bool dual;

};
