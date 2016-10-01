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
// Created by simon on 2016-01-15.
//

#ifndef BUBBA_3D_IJOYSTICKTRANSLATION_H
#define BUBBA_3D_IJOYSTICKTRANSLATION_H

class IJoystickTranslation{
public:
    enum Button : int {A,B,X,Y,LB,RB,START,BACK,GUIDE,LEFT_THUMBSTICK_PUSH,RIGHT_THUMBSTICK_PUSH};
    enum Axis : int {LEFT_THUMBSTICK_X,LEFT_THUMBSTICK_Y,RIGHT_THUMBSTICK_X,RIGHT_THUMBSTICK_Y,DPAD_X,DPAD_Y,LT,RT};

    virtual float getAxisValue(Axis axis) = 0;
    virtual float getButtonValue(Button button) = 0;
    virtual bool isDefaultMapping() = 0;

};

#endif //BUBBA_3D_IJOYSTICKTRANSLATION_H
