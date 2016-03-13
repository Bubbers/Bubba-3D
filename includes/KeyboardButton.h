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
