//
// Created by simon on 2015-12-22.
//

#include <JoystickAxis.h>
#include <SFML/Window.hpp>
#include "Input.h"
#include <cmath>

JoystickAxis::JoystickAxis(sf::Joystick::Axis axis, bool dual) : Input(dual){
    this->axisPos = axis;
}

ControlStatus::Activator JoystickAxis::getActivator() {
    return ControlStatus::JOYSTICK;
}

JoystickAxis::JoystickAxis(sf::Joystick::Axis axis, Separation separation) : Input(false) {
    this->axisPos = axis;
    this->separation = separation;
    separated = true;
}
JoystickAxis::JoystickAxis(sf::Joystick::Axis axisPos, sf::Joystick::Axis axisNeg) : Input(true) {
    this->axisPos = axisPos;
    this->axisNeg = axisNeg;
    doubleAxed = true;
}

ControlStatus JoystickAxis::getStatus() {
    if(separated)
        return getStatusSeparated();
    else
        return getStatusNormal();
}

ControlStatus JoystickAxis::getStatusSeparated() {
    ControlStatus buttonsPressed = ControlStatus();
    for(unsigned int i = 0; i < sf::Joystick::Count; i++)
        if(sf::Joystick::isConnected(i) && sf::Joystick::hasAxis(i,axisPos)) {
            float val = sf::Joystick::getAxisPosition(i, axisPos);
            if(separation == NEGATIVE && val < 0.0f)
                val = -val;
            else if(!(separation == POSITIVE && val > 0.0f))
                val = 0.0f;
            buttonsPressed.addButton(ControlStatus::activatorFromJoystickNumber(i),removeDeadZone(val));
        }
    return buttonsPressed;
}

ControlStatus JoystickAxis::getStatusNormal() {
    ControlStatus buttonsPressed = ControlStatus();
    for(unsigned int i = 0; i < sf::Joystick::Count; i++)
        if(sf::Joystick::isConnected(i) && sf::Joystick::hasAxis(i,axisPos)) {
            float val = sf::Joystick::getAxisPosition(i, axisPos);
            if(!dual)
                val = (val+100)/2; //transform [-100,100] to [0,100]
            else if(doubleAxed) {
                val = (val+100)/2; //pos should be [0,100]
                val -= (sf::Joystick::getAxisPosition(i, axisNeg) + 100)/2; //neg should be [0,-100]
            }
            buttonsPressed.addButton(ControlStatus::activatorFromJoystickNumber(i),removeDeadZone(val));
        }
    return buttonsPressed;
}


float JoystickAxis::removeDeadZone(float val) {
    if(std::abs(val) <= 15.0f)
        return 0.0f;
    val += val < 0.0f ? 15.0f : -15.0f;
    return val*(100.0f/85.0f);
}