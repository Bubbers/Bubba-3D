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