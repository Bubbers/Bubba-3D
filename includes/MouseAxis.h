//
// Created by simon on 2016-01-01.
//

#ifndef BUBBA_3D_MOUSEAXIS_H
#define BUBBA_3D_MOUSEAXIS_H

#include <Input.h>

/**
 * An input class that is activated when the mouse isn't in the center.
 * Use this input type with care. If you want this input to behave as
 * in a typical FPS game you have to reset the mouse position to the center
 * of the screen between every frame. If you want the raw mouse position
 * use MousePosition instead.
 *
 * \see MousePosition
 */
class MouseAxis : public Input {
public:

    /**
     * Represents the mouse x/y offsets from the center of the window
     */
    enum Axis {X,Y};

    /**
     * Creates a dual input which is activated when the mouse isn't
     * in the center of the window. The control status value will be
     * the offsetFromCenter*accelerator.
     */
    MouseAxis(Axis,float accelerator);
    ControlStatus getStatus();
    ControlStatus::Activator getActivator();

private:
    Axis axis;
    float accelerator;
    float clip(float n);
};


#endif //BUBBA_3D_MOUSEAXIS_H
