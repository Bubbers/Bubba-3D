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
