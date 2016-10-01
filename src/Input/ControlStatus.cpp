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
// Created by simon on 2015-12-20.
//

#include "ControlStatus.h"
#include <map>
#include <cmath>

const float ControlStatus::NO_MAX = -10000.0f;

ControlStatus::ControlStatus() {
}

ControlStatus::ControlStatus(Activator activator, float value) {
    this->addButton(activator,value);
}

void ControlStatus::addButton(Activator activator, float value) {
    buttons.insert(std::pair<Activator,int>(activator,value));
    maxValue = NO_MAX;
}

void ControlStatus::merge(ControlStatus cs) {
    for(auto it = cs.buttons.begin(); it != cs.buttons.end(); it++)
        buttons.insert(std::pair<Activator,float>(it->first,it->second));
    maxValue = NO_MAX;
}

ControlStatus::Activator ControlStatus::activatorFromJoystickNumber(int n) {
    if(n >= 8)
        throw "Argument must be less than 8";
    const Activator map[8] = {JOYSTICK_0,JOYSTICK_1,JOYSTICK_2,JOYSTICK_3,JOYSTICK_4,JOYSTICK_5,JOYSTICK_6,JOYSTICK_7};
    return map[n];
}

float ControlStatus::getValue() {
    if(maxValue == NO_MAX) { //cache max value
        maxValue = 0.0f;
        for (auto it = buttons.begin(); it != buttons.end(); it++)
            if (std::abs(it->second) > std::abs(maxValue)) {
                maxValue = it->second;
                maxValueActivator = it->first;
            }
    }
    return maxValue;
}

ControlStatus::Activator ControlStatus::getMaxActivator(){
    if(maxValue == NO_MAX)
        getValue();
    return maxValueActivator;
}

float ControlStatus::getValue(Activator activator) {
    std::map<Activator ,float>::iterator elem = buttons.find(activator);
    return elem == buttons.end() ? 0.0f : elem->second;
}

bool ControlStatus::isActive(){
    float val = getValue();
    return val != 0.0f;
}

bool ControlStatus::isActive(Activator activator) {
    float val = getValue(activator);
    return val != 0.0f;
}
