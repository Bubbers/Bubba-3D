//
// Created by simon on 2016-01-14.
//

#ifndef BUBBA_3D_JOYSTICKTRANSLATION_H
#define BUBBA_3D_JOYSTICKTRANSLATION_H

#include <SFML/Window.hpp>
#include <map>
#include <functional>
#include <IJoystickTranslation.h>

class JoystickTranslation : public IJoystickTranslation{

public:
    typedef std::function<float (unsigned int)> valueRetriever;

    JoystickTranslation(unsigned int joystickID, bool defaultMapping);
    unsigned int getJoystickID();
    float getAxisValue(Axis axis);
    float getButtonValue(Button button);
    bool isDefaultMapping();

    static valueRetriever buttonValueRetriever( unsigned int button);
    static valueRetriever axisValueRetriever( sf::Joystick::Axis axis);
    static valueRetriever axisFromButtonsRetriever(unsigned int buttonPos,unsigned int buttonNeg);
    static valueRetriever axisFromButtonRetriever(unsigned int button);
    static valueRetriever buttonFromAxisRetriever(sf::Joystick::Axis axis);
    static valueRetriever buttonFromHalfAxisRetriever(sf::Joystick::Axis axis, bool positiveElseNegative);
    static valueRetriever valRetriever(float retrieve);

    void addButton(Button,valueRetriever);
    void addAxis(Axis,valueRetriever);

private:
    unsigned int joystickID;
    bool defaultMapping;

    std::map<Button,valueRetriever> xboxToGenericButton;
    //The boolean says if the axes have their negative value to the same direction as the xbox controller
    std::map<Axis,valueRetriever> xboxToGenericAxis;

};


#endif //BUBBA_3D_JOYSTICKTRANSLATION_H
