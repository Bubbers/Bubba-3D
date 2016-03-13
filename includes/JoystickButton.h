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
