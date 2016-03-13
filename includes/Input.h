//
// Created by simon on 2015-12-20.
//

#ifndef BUBBA_3D_INPUT_H
#define BUBBA_3D_INPUT_H

#include <ControlStatus.h>

/**
 * Base class for classes that supply the status
 * for bindings to InputManager. Any class that wants
 * to work as an input binding must extend this class.
 */
class Input {
public:

    virtual ControlStatus getStatus() = 0;
    virtual ControlStatus::Activator getActivator() = 0;
    bool isDual();

protected:
    Input(bool dual);
    bool dual;

};


#endif //BUBBA_3D_INPUT_H
