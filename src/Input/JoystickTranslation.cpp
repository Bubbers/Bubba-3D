//
// Created by simon on 2016-01-14.
//

#include <JoystickTranslation.h>
#include <IJoystickTranslation.h>

JoystickTranslation::JoystickTranslation(unsigned int joystickID, bool defaultMapping) {
    this->joystickID = joystickID;
    this->defaultMapping = defaultMapping;
}
unsigned int JoystickTranslation::getJoystickID() {
    return joystickID;
}

void JoystickTranslation::addAxis(IJoystickTranslation::Axis axis, valueRetriever valRetrieve) {
    xboxToGenericAxis.insert(xboxToGenericAxis.end(),std::pair<Axis,valueRetriever>(axis,valRetrieve));
}

void JoystickTranslation::addButton(IJoystickTranslation::Button button, valueRetriever valRetrieve) {
    xboxToGenericButton.insert(xboxToGenericButton.end(),std::pair<Button,valueRetriever>(button,valRetrieve));
}

float JoystickTranslation::getAxisValue(IJoystickTranslation::Axis axis) {
    auto elem = xboxToGenericAxis.find(axis);
    if(elem == xboxToGenericAxis.end())
        return 0.0f;
    else
        return elem->second(joystickID);
}

float JoystickTranslation::getButtonValue(IJoystickTranslation::Button button) {
    auto elem = xboxToGenericButton.find(button);
    if(elem == xboxToGenericButton.end())
        return 0.0f;
    else
        return elem->second(joystickID);
}

JoystickTranslation::valueRetriever JoystickTranslation::valRetriever(float retrieve) {
    return [retrieve](unsigned int x) -> float { return retrieve; };
}

JoystickTranslation::valueRetriever JoystickTranslation::buttonValueRetriever(unsigned int button) {
    return [button](unsigned int id) -> float {return sf::Joystick::isButtonPressed(id,button) ? 100.0f : 0.0f;};
}

JoystickTranslation::valueRetriever JoystickTranslation::axisValueRetriever(sf::Joystick::Axis axis) {
    return [axis](unsigned int id) -> float {return sf::Joystick::getAxisPosition(id,axis);};
}

JoystickTranslation::valueRetriever JoystickTranslation::axisFromButtonsRetriever(unsigned int buttonPos, unsigned int buttonNeg) {
    return [buttonNeg,buttonPos](unsigned int id) -> float {
        float val = sf::Joystick::isButtonPressed(id,buttonPos) ? 100.0f : 0.0f;
        val -= sf::Joystick::isButtonPressed(id,buttonNeg) ? 100.0f : 0.0f;
        return val;
    };
}

JoystickTranslation::valueRetriever JoystickTranslation::buttonFromAxisRetriever(sf::Joystick::Axis axis) {
    return [axis](unsigned int id) -> float {return (sf::Joystick::getAxisPosition(id,axis)+100.0f)/2.0f;};
}

JoystickTranslation::valueRetriever JoystickTranslation::buttonFromHalfAxisRetriever(sf::Joystick::Axis axis,
                                                                                     bool positiveElseNegative) {
    return [axis,positiveElseNegative](unsigned int id) -> float {
        float val = sf::Joystick::getAxisPosition(id,axis);
        if(val > 0.0f && positiveElseNegative)
            return val;
        else if(val < 0.0f && !positiveElseNegative)
            return val * -1;
        else
            return 0.0f;
    };
}

JoystickTranslation::valueRetriever JoystickTranslation::axisFromButtonRetriever(unsigned int button) {
    return [button](unsigned int id) -> float {return sf::Joystick::isButtonPressed(id,button) ? 100.0f : -100.0f;};
}

bool JoystickTranslation::isDefaultMapping() {
    return defaultMapping;
}