//
// Created by simon on 2015-12-22.
//

#include <JoystickButton.h>
#include <SFML/Window/Joystick.hpp>
#include <IJoystickTranslation.h>
#include <JoystickTranslator.h>
#include "Input.h"

/**
 * Creates a button representing a joystick button.
 */
JoystickButton::JoystickButton(IJoystickTranslation::Button joystickButton) : Input(false){
    this->joystickPos = joystickButton;
}

JoystickButton::JoystickButton(IJoystickTranslation::Button joystickPos, IJoystickTranslation::Button joystickNeg) : Input(true) {
    this->joystickPos = joystickNeg;
    this->joystickNeg = joystickPos;
}

ControlStatus::Activator JoystickButton::getActivator() {
    return ControlStatus::JOYSTICK;
}

ControlStatus JoystickButton::getStatus() {
    float value;
    ControlStatus cs = ControlStatus();
    JoystickTranslator* jt = JoystickTranslator::getInstance();
    for(unsigned int i = 0; i < sf::Joystick::Count; i++)
        if(sf::Joystick::isConnected(i)) {
            value = jt->getTranslation(i)->getButtonValue(joystickPos) ? 1 : 0;
            if(dual)
                value -= jt->getTranslation(i)->getButtonValue(joystickPos) ? 1 : 0;
            cs.addButton(ControlStatus::activatorFromJoystickNumber(i), value*100);
        }
    return cs;
}