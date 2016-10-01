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

#ifndef BUBBA_3D_MOUSEBUTTON_H
#define BUBBA_3D_MOUSEBUTTON_H

#include <SFML/Window.hpp>
#include <ControlStatus.h>
#include <Input.h>

/**
 * An input class activated by mouse clicks. Mouse buttons supported
 * are left-, scroll-, right-click and two extra buttons.
 */
class MouseButton : public Input {
public:
    /**
     * Creates a non-dual button activated when \p button is pressed.
     */
    MouseButton(sf::Mouse::Button button);
    /**
     * Creates a dual button activated when either button is pressed. When \p buttonNeg is
     * pressed the ControlStatus value is -100, when \p buttonPos is pressed the value is 100. If
     * both or none are pressed the value is 0.
     */
    MouseButton(sf::Mouse::Button buttonPos, sf::Mouse::Button buttonNeg);
    ControlStatus getStatus();
    ControlStatus::Activator getActivator();

private:
    sf::Mouse::Button mButtonPos, mButtonNeg;
};


#endif //BUBBA_3D_MOUSEBUTTON_H
