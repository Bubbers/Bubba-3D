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