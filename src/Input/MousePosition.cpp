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
// Created by simon on 2015-12-29.
//

#include <MousePosition.h>
#include <SFML/Window.hpp>
#include <GL/freeglut_std.h>
#include <Globals.h>

MousePosition::MousePosition() {
    update();
}

void MousePosition::update(){
    sf::Vector2i pos = sf::Mouse::getPosition();
    x = pos.x;
    y = pos.y;
    wx = Globals::get(Globals::Key::MOUSE_WINDOW_X);
    wy = Globals::get(Globals::Key::MOUSE_WINDOW_Y);
}

int MousePosition::getRawX() {
    return x;
}

int MousePosition::getRawY() {
    return y;
}

int MousePosition::getWindowX() {
    return wx;
}

int MousePosition::getWindowY() {
    return wy;
}

int MousePosition::getXMidOffset() {
    return getWindowX() - Globals::get(Globals::Key::WINDOW_WIDTH)/2;
}

int MousePosition::getYMidOffset() {
    return getWindowY() - Globals::get(Globals::Key::WINDOW_HEIGHT)/2;
}