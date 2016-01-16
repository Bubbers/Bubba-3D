// Created by simon on 2015-12-20.
//

#ifndef BUBBA_3D_CONTROLSMANAGER_H
#define BUBBA_3D_CONTROLSMANAGER_H

#include <Input.h>
#include <vector>

typedef std::map<int,std::vector<Input*>> functionCollection ;

/*
 * \brief Used to handle player controls.
 *
 * This class is used to handle the controls of the game. A short explanation
 * of how to use the class is used is given here. You can get the singleton by calling
 * getInstance().
 *
 * The bindings can be a dual binding. This means that a function has a negative
 * and positive meaning. This is a very common concept for example turning right/left
 * or accelerating forwards/backwards. A joystick axis is naturally dual but to make
 * keyboard buttons dual you have to specify two buttons. An example of a turning function
 * (controlled with A/D or the joystick axis X) and an acceleration function (controlled
 * by W/S or the joystick axis Y) is given below. We also add a jumping button as an example
 * of a non-dual button.
 *
 * \code{.cpp}
 * //Global enum defined elsewhere:
 * enum Controls : int {TURN,ACCELERATE,JUMP};
 *
 * ControlsManager cm = ControlsManager::getInstance();
 * cm->addBindings(ACCELERATE,{new KeyboardButton(sf::Keyboard::S,sf::Keyboard::W),new JoystickAxis(sf::Joystick::Axis::Y,true)});
 * cm->addBindings(TURN,{new KeyboardButton(sf::Keyboard::D,sf::Keyboard::A),new JoystickAxis(sf::Joystick::Axis::X,true)});
 * cm->addBindings(JUMP,{new KeyboardButton(sf::Keyboard::Space),new JoystickButton(0)});
 * \endcode
 *
 * The boolean true in the second argument to the joystick axis says that the joystick is truly dual.
 * If the keyboard buttons and the joystick steers in different directions just switch there order in the parameters
 * ((sf::Keyboard::D,sf::Keyboard::A) -> (sf::Keyboard::A,sf::Keyboard::D)).
 *
 * \warning A function can only have one assignment from one device. The turn function can't have both A/D
 *          and arrow left/arrow right as both are on the keyboard. The latest specified overrides the old.
 */
class ControlsManager {
public:

    /**
     * \brief Adds a button to specified function.
     *
     * \see addBindings()
     */
    void addBinding(int function, Input* button);

    /**
     * Add several buttons to a function at once. This can also be used to
     * rebind buttons. If a function already has a keyboard button and you
     * add a new keyboard button, then the old one is removed. Any function
     * can only have all dual buttons or all non-dual buttons. If a function
     * is given buttons with different duality an exception is thrown with
     * an error message.
     *
     * \throws std::string If a function is given buttons of different dualities.
     *
     * \warning A function can only have one assignment from one device. The turn function can't have both A/D
     *          and 'arrow left'/'arrow right' on the keyboard
     */
    void addBindings(int function, std::initializer_list<Input*> buttons);

    /**
     * Clears all function bindings. This should only be used when the state of the game has
     * changed and the controls are completely different. If you want to rebind a function
     * use addBindings()
     *
     * \see addBindings()
     */
    void clearBindings();

    /**
     * Gets the status for the specified function. To see more on how to use
     * the ControlStatus see the class documentation for ControlStatus.
     *
     * \see ControlStatus
     */
    ControlStatus getStatus(int function);

    /**
     * \brief Gets the singleton instance of the controls manager.
     */
    static ControlsManager* getInstance();

private:
    ControlsManager();
    ~ControlsManager();
    functionCollection functions;
    void addBindingsForNewFunction(int function, std::initializer_list<Input*> buttons);
    void addBindingsForExistingFunction(int function, std::initializer_list<Input*> buttons,
                                        std::vector<Input*> bindings);
    void removeBindingsWithActivator(std::vector<Input*> *bindings,ControlStatus::Activator toRemove);
};


#endif //BUBBA_3D_CONTROLSMANAGER_H
