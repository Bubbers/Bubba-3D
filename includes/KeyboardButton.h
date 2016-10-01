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

#ifndef BUBBA_3D_KEYBOARDBUTTON_H
#define BUBBA_3D_KEYBOARDBUTTON_H

#include <SFML/Window.hpp>
#include <ControlStatus.h>
#include <Input.h>

/**
 * An Input class activated by keyboard buttons
 */
class KeyboardButton : public Input {
public:
    /**
     * A non-dual input activated if \p key is pressed.
     */
    KeyboardButton(sf::Keyboard::Key key);

    ~KeyboardButton() = default;
    /**
     * Creates a dual button activated when either key is pressed. When \p keyNeg is
     * pressed the ControlStatus value is -100, when ±p keyPos is pressed the value is 100. If
     * both or none are pressed the value is 0.
     */
    KeyboardButton(sf::Keyboard::Key keyPos, sf::Keyboard::Key keyNeg);
    ControlStatus getStatus();
    ControlStatus::Activator getActivator();

private:
    sf::Keyboard::Key keyPos,keyNeg;

};


#endif //BUBBA_3D_KEYBOARDBUTTON_H
