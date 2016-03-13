//
// Created by simon on 2016-01-14.
//

#ifndef BUBBA_3D_JOYSTICKTRANSLATOR_H
#define BUBBA_3D_JOYSTICKTRANSLATOR_H

#include <IJoystickTranslation.h>
#include <JoystickTranslation.h>
#include <rapidjson/document.h>

using namespace rapidjson;
using namespace std;

class ControlStatus;

class JoystickTranslator {

public:

    IJoystickTranslation* getTranslation(unsigned int joystickID);
    static JoystickTranslator* getInstance();
    void init(string filePath);
    void updateMapping();
    ~JoystickTranslator();

private:
    Document *jsonConfig;
    JoystickTranslator();
    std::vector<JoystickTranslation> translations;
    void readDocument(Document *doc);
    void check(bool,std::string);
    int getJoystick(int devId,int vendId, unsigned int startAt);
    void readMappings(Value* mappings,unsigned int joystickID, bool defaultMapping);
    JoystickTranslation::valueRetriever decideOnButtonRetriever(Value* mapData);
    JoystickTranslation::valueRetriever decideOnAxisRetriever(Value* mapData, string axisName);

    static IJoystickTranslation::Button buttonFromString(std::string name);
    static IJoystickTranslation::Axis axisFromString(std::string name);
    static sf::Joystick::Axis SFMLAxisFromString(std::string name);

};


#endif //BUBBA_3D_JOYSTICKTRANSLATOR_H
