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

#include <JoystickAxis.h>
#include <SFML/Window.hpp>
#include "Input.h"
#include <cmath>
#include <JoystickTranslation.h>
#include <JoystickTranslator.h>

JoystickAxis::JoystickAxis(JoystickTranslation::Axis axis, bool dual) : Input(dual){
    this->axisPos = axis;
}

ControlStatus::Activator JoystickAxis::getActivator() {
    return ControlStatus::JOYSTICK;
}

JoystickAxis::JoystickAxis(IJoystickTranslation::Axis axis, Separation separation) : Input(false) {
    this->axisPos = axis;
    this->separation = separation;
    separated = true;
}
JoystickAxis::JoystickAxis(IJoystickTranslation::Axis axisPos, IJoystickTranslation::Axis axisNeg) : Input(true) {
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
    JoystickTranslator* jt = JoystickTranslator::getInstance();
    for(unsigned int i = 0; i < sf::Joystick::Count; i++)
        if(sf::Joystick::isConnected(i)) {
            float val = jt->getTranslation(i)->getAxisValue(axisPos);
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
    JoystickTranslator* jt = JoystickTranslator::getInstance();
    for(unsigned int i = 0; i < sf::Joystick::Count; i++)
        if(sf::Joystick::isConnected(i)) {
            float val = jt->getTranslation(i)->getAxisValue(axisPos);
            if(!dual)
                val = (val+100)/2; //transform [-100,100] to [0,100]
            else if(doubleAxed) {
                val = (val+100)/2; //pos should be [0,100]
                val -= (jt->getTranslation(i)->getAxisValue(axisNeg) + 100)/2; //neg should be [0,-100]
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