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
// Created by simon on 2015-12-22.
//

#ifndef BUBBA_3D_JOYSTICKBUTTON_H
#define BUBBA_3D_JOYSTICKBUTTON_H

#include <ControlStatus.h>
#include <Input.h>
#include <IJoystickTranslation.h>

/**
 * An input class for joystick buttons.
 */
class JoystickButton : public Input{
public:
    /**
     * Creates a non-dual button activated when button number \p button is pressed.
     */
    JoystickButton(IJoystickTranslation::Button button);
    /**
     * Creates a dual button activated when either button is pressed. When \p neg is
     * pressed the ControlStatus value is -100, when \p pos is pressed the value is 100. If
     * both or none are pressed the value is 0.
     */
    JoystickButton(IJoystickTranslation::Button pos, IJoystickTranslation::Button neg);
    ControlStatus getStatus();
    ControlStatus::Activator getActivator();

private:
    IJoystickTranslation::Button joystickPos,joystickNeg;

};


#endif //BUBBA_3D_JOYSTICKBUTTON_H
