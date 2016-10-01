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

#include <MouseButton.h>
#include "Input.h"
#include <SFML/Window.hpp>

MouseButton::MouseButton(sf::Mouse::Button button) : Input(false){
    this->mButtonPos = button;
}

MouseButton::MouseButton(sf::Mouse::Button pos, sf::Mouse::Button neg) : Input(true) {
    this->mButtonPos = pos;
    this->mButtonNeg = neg;
}

ControlStatus MouseButton::getStatus() {
    float value = sf::Mouse::isButtonPressed(mButtonPos) ? 1.0f : 0.0f;
    if(dual)
        value -= sf::Mouse::isButtonPressed(mButtonPos) ? 1.0f : 0.0f;
    return ControlStatus(ControlStatus::MOUSE, value*100.0f);
}

ControlStatus::Activator MouseButton::getActivator() {
    return ControlStatus::MOUSE;
}