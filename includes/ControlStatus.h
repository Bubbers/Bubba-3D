//
// Created by simon on 2015-12-20.
//

#ifndef BUBBA_3D_BUTTONSTATUS_H
#define BUBBA_3D_BUTTONSTATUS_H

#include <map>

/**
 * \brief The class that contains information about a function at the
 * state of creation.
 *
 * To read on how to make bindings check the documentation for ControlsManager.
 *
 * Checking if a control after the bindings has been set is very simple.
 * We will use the same bindings as given in the example in ControlsManager.
 * The code will make sure that the vehicle does what the user wants.
 *
 * \code{.cpp}
 * //Global enum defined elsewhere (Same enum as when we made the bindings):
 * enum Controls : int {TURN,ACCELERATE,JUMP};
 *
 * ControlsManager cm = ControlsManager::getInstance();
 *
 * ControlStatus cs = cm->getStatus(ACCELERATE);
 * if(cs.isActive()){
 *     //increase the speed of the vehicle
 *     accelerateVehicle(cs->getValue()*maxAcceleration);
 * }
 *
 * cs = cm->getStatus(TURN);
 * if(cs.isActive()){
 *     turnVehicle(cs->getValue()*maxTurnSpeed);
 * }
 *
 * if(cm->getStatus(JUMP).isActive()){
 *     jump();
 * }
 *
 * \endcode
 */
class ControlStatus {
public:
    enum Activator  {NONE = 1024,KEYBOARD = 256, MOUSE = 512, JOYSTICK_0 = 1, JOYSTICK_1 = 2, JOYSTICK_2 = 4,
                    JOYSTICK_3 = 8, JOYSTICK_4 = 16, JOYSTICK_5 = 32, JOYSTICK_6 = 64, JOYSTICK_7 = 128, JOYSTICK = 255};


    static Activator activatorFromJoystickNumber(int n);

    /**
     * Returns the activator that produced the result given by
     * getValue() and isActive()
     */
    Activator getMaxActivator();

    /**
     * Returns a number in the range [-100,100] for dual buttons.
     * Returns a number in the range [0,100] for non-dual buttons.
     * Returns the maximum value from any activator.
     */
    float getValue();

    /**
     * Gets the value from the specified activator.
     * Returns a number in the range [-100,100] for dual buttons.
     * Returns a number in the range [0,100] for non-dual buttons.
     */
    float getValue(Activator activator);

    /*
     * Alias for getValue() != 0;
     */
    bool isActive();

    /**
     * Alias for:
     * return abs(getValue(activator)) != 0;
     */
    bool isActive(Activator activator);

    /**
     * Creates a ControlStatus activated by \p activator with value \p value.
     */
    ControlStatus(Activator activator, float value);
    ControlStatus();
    void addButton(Activator activator, float value);
    void merge(ControlStatus cs);

private:
    std::map<Activator,float> buttons;
    float maxValue = NO_MAX;
    Activator maxValueActivator;
	static const float NO_MAX;

};


#endif //BUBBA_3D_BUTTONSTATUS_H
