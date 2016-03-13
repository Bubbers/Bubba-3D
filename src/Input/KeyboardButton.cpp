//
// Created by simon on 2015-12-22.
//

#include <KeyboardButton.h>
#include "Input.h"
#include <SFML/Window.hpp>

KeyboardButton::KeyboardButton(sf::Keyboard::Key key) : Input(false){
    this->keyPos = key;
}

KeyboardButton::KeyboardButton(sf::Keyboard::Key keyPos, sf::Keyboard::Key keyNeg) : Input(true){
    this->keyNeg = keyNeg;
    this->keyPos = keyPos;
}

ControlStatus::Activator KeyboardButton::getActivator() {
    return ControlStatus::KEYBOARD;
}

ControlStatus KeyboardButton::getStatus() {
    float value = sf::Keyboard::isKeyPressed(keyPos) ? 100.0f : 0.0f;
    if(dual)
        value -= sf::Keyboard::isKeyPressed(keyNeg) ? 100.0f : 0.0f;
    return ControlStatus(ControlStatus::KEYBOARD,value);
}