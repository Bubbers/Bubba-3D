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
